#include "networkmanager.h"
#include "mainwindow.h"


/**
 * @brief NetworkManager::NetworkManager
 * TCP example - https://www.bogotobogo.com/cplusplus/sockets_server_client_QT.php
 * HTTP example - https://code.qt.io/cgit/qt/qtbase.git/tree/examples/network/http?h=5.13
 * Download Manager example
 */

NetworkManager* NetworkManager::instance_ = nullptr;

/**
 * @brief NetworkManager::NetworkManager
 * Network manager constructor
 */
NetworkManager::NetworkManager()
{
    socket_ = new QTcpSocket;
    connect(socket_, &QTcpSocket::connected, this, &NetworkManager::connected);
    connect(socket_, &QTcpSocket::disconnected, this, &NetworkManager::disconnected);
    connect(socket_, &QIODevice::readyRead, this, &NetworkManager::readJson);
    connect(socket_, QOverload<QAbstractSocket::SocketError>::of(&QAbstractSocket::error), this, &NetworkManager::handleError);

    url_ = "http://34.66.244.22:8000"; // Server: 34.66.244.22
//    url_ = "http://localhost:5000";
}

/**
 * @brief NetworkManager::getInstance
 * Gets the singleton instance of the Network Manager
 * @return Instance of the Network Manager
 */
NetworkManager* NetworkManager::getInstance() {
    if(instance_ == nullptr) {
        // Lazy instantiation
        instance_ = new NetworkManager;
    }

    return instance_;
}

/**
 * @brief NetworkManager::Post
 * Posts to a JSON request to the server
 * @param request
 * @param toPost
 */
void NetworkManager::Post(QNetworkRequest *request, QByteArray *toPost) {
    request->setHeader(QNetworkRequest::ContentLengthHeader, toPost->length()); // TODO
    request->setHeader(QNetworkRequest::ContentTypeHeader, "application/json");
    manager_.post(*request, *toPost);
}

/**
 * @brief NetworkManager::SignInPost
 * Handles a sign in post request
 * @param toPost Json to post
 */
void NetworkManager::SignInPost(QByteArray *toPost) {
    connect(&manager_, &QNetworkAccessManager::finished, this, &NetworkManager::onSignInManagerFinished);
    QUrl postUrl = url_ + "/users/signin";
    qDebug() << *toPost;
    QNetworkRequest request(postUrl); // replace w/ post url
    Post(&request, toPost);
}

/**
 * @brief NetworkManager::SignUpPost
 * Handles a sign up post request
 * @param toPost Json to post
 */
void NetworkManager::SignUpPost(QByteArray *toPost) {
    connect(&manager_, &QNetworkAccessManager::finished, this, &NetworkManager::onSignUpManagerFinished);
    QUrl postUrl = url_ + "/users/signup";
    qDebug() << postUrl;
    QNetworkRequest request(postUrl); // replace w/ post url
    Post(&request, toPost);
}

/**
 * @brief NetworkManager::SignOutPost
 * Handles sign out post request
 * @param toPost Json to post
 */
void NetworkManager::SignOutPost(QByteArray *toPost) {
    connect(&manager_, &QNetworkAccessManager::finished, this, &NetworkManager::onSignOutManagerFinished);
    QUrl postUrl = url_ + "/users/signout";
    QNetworkRequest request(postUrl); // replace w/ post url
    Post(&request, toPost);
}

/**
 * @brief NetworkManager::FilePost
 * Handles a file POST to the server
 * @param filePath File path to the file to post
 * @param DirectoryAddedTo Directory the file was added to
 */
bool NetworkManager::FilePost(QString filePath, QString directory) {
    connect(&manager_, &QNetworkAccessManager::finished, this, &NetworkManager::onFileManagerFinished);
    QUrl postUrl = url_ + "/file/";
    QNetworkRequest request(postUrl);

    QString dropbucketDirPath = QDir::homePath() + "/Dropbucket";

    QStringList splitPath = filePath.split("/", QString::SkipEmptyParts);
    QString newFilePath = dropbucketDirPath + directory + splitPath[splitPath.size() - 1];
    QString relativePath = newFilePath;
    relativePath.remove(0, dropbucketDirPath.length());

    QFile fileToSave(newFilePath);
    QFile *file = new QFile(filePath);

    file->open(QIODevice::ReadOnly);
    fileToSave.resize(file->size());
    QByteArray data = file->readAll();
    fileToSave.open(QIODevice::WriteOnly);
    fileToSave.write(data);
    file->close();
    fileToSave.close();

    QHttpMultiPart *multiPart = new QHttpMultiPart(QHttpMultiPart::FormDataType);

    QHttpPart deviceIdPart;
    deviceIdPart.setHeader(QNetworkRequest::ContentDispositionHeader, QVariant("form-data; name=\"device_id\""));
    deviceIdPart.setBody(QSysInfo::machineUniqueId());

    QHttpPart relativePathPart;
    relativePathPart.setHeader(QNetworkRequest::ContentDispositionHeader, QVariant("form-data; name=\"relative_path\""));
    relativePathPart.setBody(relativePath.toUtf8());

    QHttpPart filePart;
    file->open(QIODevice::ReadOnly);
    filePart.setHeader(QNetworkRequest::ContentDispositionHeader, QVariant("form-data; name=\"file\"; filename=\"/" + filePath +"\""));
    filePart.setBodyDevice(file);
    file->setParent(multiPart);

    multiPart->append(deviceIdPart);
    multiPart->append(relativePathPart);
    multiPart->append(filePart);

    // https://stackoverflow.com/questions/39549211/qhttpmultipart-generates-different-boundary
    quint32 random[6];
    QRandomGenerator::global()->fillRange(random);
    QByteArray boundary = "--boundary_zyl_"
               + QByteArray::fromRawData(reinterpret_cast<char *>(random),
    sizeof(random)).toBase64();

    QByteArray contentType;
    contentType += "multipart/";
    contentType += "form-data";
    contentType += "; boundary=";
    contentType += boundary;
    multiPart->setBoundary(boundary);
    request.setHeader(QNetworkRequest::ContentTypeHeader, contentType);
    QNetworkReply *reply = manager_.post(request, multiPart);
    multiPart->setParent(reply);
//    file->close();

    return true;
}

/**
 * @brief NetworkManager::GetFile
 * Gets a file from the server
 * @param filePath The file path for the parameters in the get
 */
void NetworkManager::FileGet(QString filePath) {
    connect(&manager_, &QNetworkAccessManager::finished, this, &NetworkManager::onGetFileManagerFinished);
    QUrl getUrl = url_ + "/file/?relative_path=" + "test.txt";
    qDebug() << getUrl;
    QNetworkRequest request(getUrl);
    reply_ = manager_.get(request);
    connect(reply_, &QNetworkReply::downloadProgress, this, &NetworkManager::DownloadProgress);
    connect(reply_, &QNetworkReply::readyRead, this, &NetworkManager::onGetFileReadyRead);
}

/**
 * @brief NetworkManager::DownloadProgress
 * Reports the download progress
 * @param bytesReceived
 * @param bytesTotal
 */
void NetworkManager::DownloadProgress(qint64 bytesReceived, qint64 bytesTotal) {
    qDebug() << bytesReceived << bytesTotal;
}

/**
 * @brief NetworkManager::Delete
 * Deletes a file from the network
 * @param relativePath String to the relative path of the file
 */
void NetworkManager::FileDelete(QString relativePath) {
    connect(&manager_, &QNetworkAccessManager::finished, this, &NetworkManager::onDeleteFileManagerFinished);

    QUrl deleteUrl = url_ + "/file/?relative_path=" + relativePath;
    QNetworkRequest request(deleteUrl);
    manager_.deleteResource(request);
}

/**
 * @brief NetworkManager::handleError
 * Reports socket error
 * @param socketError Socket error
 */
void NetworkManager::handleError(QAbstractSocket::SocketError socketError) {
    qDebug() << socketError;
}

/**
 * @brief NetworkManager::onManagerFinished
 * Handles when the manager finishes
 * @param reply Reply from server
 */
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
 * Handles when the sign in request is completed
 * @param reply Reply from the server
 */
void NetworkManager::onSignInManagerFinished(QNetworkReply *reply) {
    QByteArray buffer = reply->readAll();
    QJsonDocument jsonDoc(QJsonDocument::fromJson(buffer));
    QJsonObject jsonReply = jsonDoc.object();

    QJsonObject status = jsonReply["status"].toObject();
    QJsonArray fileSystemArray = jsonReply["fs_objects"].toArray();
    QJsonArray directoriesArray = jsonReply["directories"].toArray();
    QString userid = jsonReply["user_id"].toString();

    QString message = jsonReply["message"].toString();
    qDebug() << status;
    qDebug() << message;
    if(message == "Sign in successful") {
        SignInSuccessfulSignal();

        socket_->connectToHost("34.66.244.22", 12000);
//        socket_->connectToHost("localhost", 12000);
        socket_->setSocketOption(QAbstractSocket::KeepAliveOption, true);

        SignInLoadSceneSignal(directoriesArray, fileSystemArray);
        userid_ = userid;
        SetUseridSignal(userid);
    }

    disconnect(&manager_, &QNetworkAccessManager::finished, this, &NetworkManager::onSignInManagerFinished);
}

/**
 * @brief NetworkManager::DownloadFiles
 * Downloads the files needed from sync
 * @param relativePaths List of the relative paths to download
 */
void NetworkManager::DownloadFiles(QStringList relativePaths) {
    connect(&manager_, &QNetworkAccessManager::finished, this, &NetworkManager::DownloadComplete);

    QString getUrl = url_ + "/file/?relative_path=";
    QStringList::Iterator it;
    for(it = relativePaths.begin(); it != relativePaths.end(); it++) {
        QNetworkRequest request(QUrl(getUrl + (*it)));
        QNetworkReply *reply = manager_.get(request);

        currentDownloads_.append(reply);
    }
}

/**
 * @brief NetworkManager::DownloadComplete
 * Handles when a download is completed
 * @param reply
 */
void NetworkManager::DownloadComplete(QNetworkReply *reply) {
    QString urlToRemove = url_ + "/file/?relative_path=";
    QString relativePath = reply->url().toString().remove(0,urlToRemove.size());
    QFile fileToWrite(QDir::homePath() + "/Dropbucket/" + relativePath);
    fileToWrite.open(QIODevice::WriteOnly);
    fileToWrite.write(reply->readAll());
    fileToWrite.close();

    currentDownloads_.removeAll(reply);
    reply->deleteLater();

    // Current downloads all done
    if(currentDownloads_.empty()) {
        disconnect(&manager_, &QNetworkAccessManager::finished, this, &NetworkManager::DownloadComplete);
        // Emit signal
        DownloadCompleteSignal();
    }
}

/**
 * @brief NetworkManager::onSignUpManagerFinished
 * Handles when a sign up request is complete
 * @param reply Reply from server
 */
void NetworkManager::onSignUpManagerFinished(QNetworkReply *reply) {
    QByteArray buffer = reply->readAll();
    QJsonDocument jsonDoc(QJsonDocument::fromJson(buffer));
    QJsonObject jsonReply = jsonDoc.object();

    QString message = jsonReply["message"].toString();
    if(message == "User successfully created! You can now sign in.") {
        SignUpSuccessfulSignal();
    }
    disconnect(&manager_, &QNetworkAccessManager::finished, this, &NetworkManager::onSignUpManagerFinished);
}

/**
 * @brief NetworkManager::onSignOutManagerFinished
 * Handles when a sign out request is complete
 * @param reply Reply from server
 */
void NetworkManager::onSignOutManagerFinished(QNetworkReply *reply) {
    QByteArray buffer = reply->readAll();
    QJsonDocument jsonDoc(QJsonDocument::fromJson(buffer));
    QJsonObject jsonReply = jsonDoc.object();


    disconnect(&manager_, &QNetworkAccessManager::finished, this, &NetworkManager::onSignOutManagerFinished);
    reply->deleteLater();
}

/**
 * @brief NetworkManager::onFileManagerFinished
 * Handles whena file POST is complete
 * @param reply Reply from server
 */
void NetworkManager::onFileManagerFinished(QNetworkReply *reply) {
    QByteArray buffer = reply->readAll();
    QJsonDocument jsonDoc(QJsonDocument::fromJson(buffer));
    QJsonObject jsonReply = jsonDoc.object();

    QString message = jsonReply["message"].toString();

    disconnect(&manager_, &QNetworkAccessManager::finished, this, &NetworkManager::onFileManagerFinished);
    reply->deleteLater();
}

/**
 * @brief NetworkManager::onGetFileManagerFinished
 * Handles when a file get is complete
 * @param reply Reply from server
 */
void NetworkManager::onGetFileManagerFinished(QNetworkReply *reply) {
    if(reply->error()) {
        qDebug() << reply->errorString();
    }
    else{
        qDebug() << "no error";
    }
    disconnect(&manager_, &QNetworkAccessManager::finished, this, &NetworkManager::onGetFileManagerFinished);
    reply->deleteLater();
}

/**
 * @brief NetworkManager::onGetFileReadyRead
 * Handles when a get file is done - this is a test function
 */
void NetworkManager::onGetFileReadyRead() {
    QString urlToRemove = url_ + "/file/?relative_path=";
    QString relativePath = reply_->url().toString().remove(0,urlToRemove.size());
    QFile fileToWrite("C:/Users/thoma/Dropbucket/test.txt");
    fileToWrite.open(QIODevice::WriteOnly);
    fileToWrite.write(reply_->readAll());
    fileToWrite.close();
}

/**
 * @brief NetworkManager::onDeleteFileManagerFinished
 * Handles when a delete file request is complete
 * @param reply Reply from server
 */
void NetworkManager::onDeleteFileManagerFinished(QNetworkReply *reply) {
    QByteArray buffer = reply->readAll();
    QJsonDocument jsonDoc(QJsonDocument::fromJson(buffer));
    QJsonObject jsonReply = jsonDoc.object();

    QString message = jsonReply["message"].toString();

    qDebug() << message;

    if(message == "File successfully deleted") {
        FileDeleteSuccessfulSignal();
    }

    disconnect(&manager_, &QNetworkAccessManager::finished, this, &NetworkManager::onDeleteFileManagerFinished);
}

/**
 * @brief NetworkManager::connected
 * Called when TCP socket connection is successful
 */
void NetworkManager::connected() {
    qDebug() << "connected";
    QString toSend = MainWindow::username + "," + QSysInfo::machineUniqueId();
    qDebug() << toSend;
    socket_->write(toSend.toUtf8());
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
 * Reads a json recieved in the socket
 */
void NetworkManager::readJson() {
    QByteArray read = socket_->readAll();
    QJsonDocument tcpJson(QJsonDocument::fromJson(read));
    QJsonObject jsonReply = tcpJson.object();

    QJsonArray fileSystemArray = jsonReply["fs_objects"].toArray();
    QJsonArray directoriesArray = jsonReply["directories"].toArray();

    SyncAndLoadSceneSignal(directoriesArray, fileSystemArray);
}
