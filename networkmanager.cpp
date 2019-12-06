#include "networkmanager.h"

/**
 * @brief NetworkManager::NetworkManager
 * TCP example - https://www.bogotobogo.com/cplusplus/sockets_server_client_QT.php
 * HTTP example - https://code.qt.io/cgit/qt/qtbase.git/tree/examples/network/http?h=5.13
 */

NetworkManager* NetworkManager::instance_ = nullptr;

/**
 * @brief NetworkManager::NetworkManager
 */
NetworkManager::NetworkManager()
{
    socket_ = new QTcpSocket;
    connect(socket_, &QTcpSocket::connected, this, &NetworkManager::connected);
    connect(socket_, &QTcpSocket::disconnected, this, &NetworkManager::disconnected);
    connect(socket_, &QIODevice::readyRead, this, &NetworkManager::readJson);
    connect(socket_, QOverload<QAbstractSocket::SocketError>::of(&QAbstractSocket::error), this, &NetworkManager::handleError);
    socket_->connectToHost("localhost", 12000); // Need host name, port
    socket_->setSocketOption(QAbstractSocket::KeepAliveOption, true);

    url = "http://localhost:5000";
}

/**
 * @brief NetworkManager::getInstance
 * @return
 */
NetworkManager* NetworkManager::getInstance() {
    if(instance_ == nullptr) {
        // Lazy instantiation
        instance_ = new NetworkManager;
    }

    return instance_;
}

/* Saved for if we need to a do a put
void NetworkManager::Put(QString urlSuffix, QFile *toPut) {
    QUrl putUrl = url + urlSuffix;
    QNetworkRequest request(putUrl); // replace with put url
    if(toPut->open(QIODevice::ReadOnly)) {
        QByteArray ba = toPut->readAll();
        manager_.put(request, ba);
    }
}
*/

/**
 * @brief NetworkManager::Post
 * @param request
 * @param toPost
 */
void NetworkManager::Post(QNetworkRequest *request, QByteArray *toPost) {
    request->setHeader(QNetworkRequest::ContentLengthHeader, toPost->length()); // TODO
    request->setHeader(QNetworkRequest::ContentTypeHeader, "application/json");
    manager_.post(*request, *toPost);
    qDebug() << "Posted";
}

/**
 * @brief NetworkManager::SignInPost
 * @param toPost
 */
void NetworkManager::SignInPost(QByteArray *toPost) {
    connect(&manager_, &QNetworkAccessManager::finished, this, &NetworkManager::onSignInManagerFinished);
    QUrl postUrl = url + "/users/signin";
    QNetworkRequest request(postUrl); // replace w/ post url
//    Post(&manager, &request, toPost);
    Post(&request, toPost);
}

/**
 * @brief NetworkManager::SignUpPost
 * @param toPost
 */
void NetworkManager::SignUpPost(QByteArray *toPost) {
    connect(&manager_, &QNetworkAccessManager::finished, this, &NetworkManager::onSignUpManagerFinished);
    QUrl postUrl = url + "/users/signup";
    qDebug() << postUrl;
    QNetworkRequest request(postUrl); // replace w/ post url
    Post(&request, toPost);
}

/**
 * @brief NetworkManager::SignOutPost
 * @param toPost
 */
void NetworkManager::SignOutPost(QByteArray *toPost) {
    connect(&manager_, &QNetworkAccessManager::finished, this, &NetworkManager::onSignOutManagerFinished);
    QUrl postUrl = url + "/users/signout";
    QNetworkRequest request(postUrl); // replace w/ post url
    Post(&request, toPost);
}

void NetworkManager::FilePost(QString filePath, QString DirectoryAddedTo) {
    qDebug() << DirectoryAddedTo;
    connect(&manager_, &QNetworkAccessManager::finished, this, &NetworkManager::onFileManagerFinished);
    QUrl postUrl = url + "/file/";
    QNetworkRequest request(postUrl);

    QString dropbucketDirPath = QDir::homePath() + "/Dropbucket/";

    QStringList splitPath = filePath.split("/", QString::SkipEmptyParts);
    QString newFilePath = dropbucketDirPath + DirectoryAddedTo + splitPath[splitPath.size() - 1];
    QString relativePath = newFilePath;
    relativePath.remove(0, dropbucketDirPath.length());

    QFile fileToSave(newFilePath);
    QFile file(filePath);
    QByteArray data;
    QByteArray imageFormat = QImageReader::imageFormat(filePath);
    qDebug() << "image format: " << imageFormat;
    if(imageFormat == "png" || imageFormat == "jpeg") {
        QImage image(filePath);
        QBuffer buff(&data);
        buff.open(QIODevice::WriteOnly);
        if(imageFormat == "png") {
            image.save(&buff, "PNG");
        }
        else if(imageFormat == "jpeg") {
            image.save(&buff, "JPG");
        }
        qDebug() << "Data: " << data;
    }
    else {
        if(file.open(QIODevice::ReadOnly)) {
            data = file.readAll();
            qDebug () << "file opened";
            qDebug() << "Data: " << data.constData();
        }
        else {
            qDebug() << "file not opened";
        }
    }

    fileToSave.open(QIODevice::WriteOnly);
    fileToSave.write(data);
    fileToSave.close();

    QJsonObject postJson;

    postJson.insert("file", data.constData());
    postJson.insert("relative_path", relativePath);
    postJson.insert("device_id", QString(QSysInfo::machineUniqueId()));
//    postJson.insert("user_id", userid_);

    QByteArray toPost;
    QJsonDocument doc(postJson);
    qDebug() << doc.toJson();
    QByteArray json = doc.toJson();
    Post(&request, &json);
}

/**
 * @brief NetworkManager::GetFile
 * @param urlSuffix
 */
void NetworkManager::FileGet(QString filePath) {
    connect(&manager_, &QNetworkAccessManager::finished, this, &NetworkManager::onGetFileManagerFinished);
    QUrl getUrl = url + "/file/?relative_path=" + "abc.png";
    qDebug() << getUrl;
    QNetworkRequest request(getUrl);
    manager_.get(request);
}

/**
 * @brief NetworkManager::Delete
 * @param urlSuffix
 * @param toDelete
 */
void NetworkManager::FileDelete(QString filePath) {
    QUrl deleteUrl = url;
    QNetworkRequest request(deleteUrl); // replace URL w/ path to the file going to be deleted
//    manager.deleteResource(request);
}

/**
 * @brief NetworkManager::handleError
 * @param socketError
 */
void NetworkManager::handleError(QAbstractSocket::SocketError socketError) {
//    qDebug() << socketError;
}

void NetworkManager::onManagerFinished(QNetworkReply *reply) {
    qDebug() << reply;
    QVariant status_code = reply->attribute(QNetworkRequest::HttpStatusCodeAttribute);
        if (status_code.isValid()) {
            // Print or catch the status code
            QString status = status_code.toString(); // or status_code.toInt();
            qDebug() << status;
        }
}

/**
 * @brief NetworkManager::onSignInManagerFinished
 * @param reply
 */
void NetworkManager::onSignInManagerFinished(QNetworkReply *reply) {
    QByteArray buffer = reply->readAll();
    qDebug() << buffer;
    QJsonDocument jsonDoc(QJsonDocument::fromJson(buffer));
    QJsonObject jsonReply = jsonDoc.object();

    QJsonObject status = jsonReply["status"].toObject();
    QJsonArray fileSystemArray = jsonReply["fs_objects"].toArray();
    QJsonArray directoriesArray = jsonReply["directories"].toArray();
    QString userid = jsonReply["user_id"].toString();

    QString message = jsonReply["message"].toString();
    qDebug() << message;
    qDebug() << fileSystemArray;
    qDebug() << directoriesArray;
    if(message == "Sign in successful") {
        SignInSuccessful();
        LoadScene(directoriesArray, fileSystemArray);
        userid_ = userid;
        SetUserid(userid);
    }

    disconnect(&manager_, &QNetworkAccessManager::finished, this, &NetworkManager::onSignInManagerFinished);
}

/**
 * @brief NetworkManager::onSignUpManagerFinished
 * @param reply
 */
void NetworkManager::onSignUpManagerFinished(QNetworkReply *reply) {
    qDebug() << "Manager finished called";
    QByteArray buffer = reply->readAll();
    qDebug() << buffer;
    QJsonDocument jsonDoc(QJsonDocument::fromJson(buffer));
    QJsonObject jsonReply = jsonDoc.object();

    QString message = jsonReply["message"].toString();
    qDebug() << message;
    if(message == "User successfully created! You can now sign in.") {
        SignUpSuccessful();
    }
    disconnect(&manager_, &QNetworkAccessManager::finished, this, &NetworkManager::onSignUpManagerFinished);
}

/**
 * @brief NetworkManager::onSignOutManagerFinished
 * @param reply
 */
void NetworkManager::onSignOutManagerFinished(QNetworkReply *reply) {
    QByteArray buffer = reply->readAll();
    qDebug() << buffer;
    QJsonDocument jsonDoc(QJsonDocument::fromJson(buffer));
    QJsonObject jsonReply = jsonDoc.object();


    disconnect(&manager_, &QNetworkAccessManager::finished, this, &NetworkManager::onSignOutManagerFinished);
}

void NetworkManager::onFileManagerFinished(QNetworkReply *reply) {
    QByteArray buffer = reply->readAll();
    qDebug() << buffer;
    QJsonDocument jsonDoc(QJsonDocument::fromJson(buffer));
    QJsonObject jsonReply = jsonDoc.object();

    QString message = jsonReply["message"].toString();
    qDebug() << message;

    disconnect(&manager_, &QNetworkAccessManager::finished, this, &NetworkManager::onFileManagerFinished);
}

void NetworkManager::onGetFileManagerFinished(QNetworkReply *reply) {
    qDebug() << "In file manager get";
    qDebug() << reply;
    QByteArray buffer = reply->readAll();
    qDebug() << buffer;
    QJsonDocument jsonDoc(QJsonDocument::fromJson(buffer));
    QJsonObject jsonReply = jsonDoc.object();

    QString data = jsonReply["data"].toString();
    qDebug() << data;
    qDebug() << "End manager";
    disconnect(&manager_, &QNetworkAccessManager::finished, this, &NetworkManager::onGetFileManagerFinished);
}

/**
 * @brief NetworkManager::connected
 * Called when TCP socket connection is successful
 */
void NetworkManager::connected() {
    qDebug() << "connceted";
}

/**
 * @brief NetworkManager::disconnected
 * Called when TCP socket is disconnected
 */
void NetworkManager::disconnected() {
    qDebug() << "disconnected";
}

/**
 * @brief NetworkManager::readJson
 */
void NetworkManager::readJson() {
    qDebug() << socket_->readAll();
    socket_->write("herro");
}
