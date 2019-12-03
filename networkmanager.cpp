#include "networkmanager.h"

/**
 * @brief NetworkManager::NetworkManager
 * TCP example - https://www.bogotobogo.com/cplusplus/sockets_server_client_QT.php
 * HTTP example - https://code.qt.io/cgit/qt/qtbase.git/tree/examples/network/http?h=5.13
 */

NetworkManager* NetworkManager::instance_ = nullptr;

NetworkManager::NetworkManager()
{
    socket_ = new QTcpSocket;
    connect(socket_, &QTcpSocket::connected, this, &NetworkManager::connected);
    connect(socket_, &QTcpSocket::disconnected, this, &NetworkManager::disconnected);
    connect(socket_, &QIODevice::readyRead, this, &NetworkManager::readJson);
    connect(socket_, QOverload<QAbstractSocket::SocketError>::of(&QAbstractSocket::error), this, &NetworkManager::handleError);
    socket_->connectToHost("localhost", 12000); // Need host name, port
    socket_->setSocketOption(QAbstractSocket::KeepAliveOption, true);

//    connect(&manager_, &QNetworkAccessManager::finished, this, &NetworkManager::onManagerFinished);
    url = "http://localhost:5000";
//    in.setDevice(socket_);
//    in.setVersion(QDataStream::Qt_5_0);
}

NetworkManager* NetworkManager::getInstance() {
    if(instance_ == nullptr) {
        // Lazy instantiation
        instance_ = new NetworkManager;
    }

    return instance_;
}

//void NetworkManager::Put(QString urlSuffix, QFile *toPut) {
//    QUrl putUrl = url + urlSuffix;
//    QNetworkRequest request(putUrl); // replace with put url
//    if(toPut->open(QIODevice::ReadOnly)) {
//        QByteArray ba = toPut->readAll();
//        manager_.put(request, ba);
//    }
//}

void NetworkManager::Post(QNetworkAccessManager *manager, QNetworkRequest *request, QByteArray *toPost) {
    request->setHeader(QNetworkRequest::ContentLengthHeader, toPost->length()); // TODO
    request->setHeader(QNetworkRequest::ContentTypeHeader, "application/json");
    manager->post(*request, *toPost);
}

void NetworkManager::SignInPost(QByteArray *toPost) {
    QNetworkAccessManager manager;
    connect(&manager, &QNetworkAccessManager::finished, this, &NetworkManager::onSignInManagerFinished);
    QUrl postUrl = url + "/users/signin";
    QNetworkRequest request(postUrl); // replace w/ post url
    Post(&manager, &request, toPost);
}

void NetworkManager::SignUpPost(QByteArray *toPost) {
    QNetworkAccessManager manager;
    connect(&manager, &QNetworkAccessManager::finished, this, &NetworkManager::onSignUpManagerFinished);
    QUrl postUrl = url + "/users/signup";
    qDebug() << postUrl;
    QNetworkRequest request(postUrl); // replace w/ post url
    Post(&manager, &request, toPost);
}

void NetworkManager::SignOutPost(QByteArray *toPost) {
    QNetworkAccessManager manager;
    connect(&manager, &QNetworkAccessManager::finished, this, &NetworkManager::onSignOutManagerFinished);
    QUrl postUrl = url + "/users/signout";
    QNetworkRequest request(postUrl); // replace w/ post url
    Post(&manager, &request, toPost);
}

void NetworkManager::GetFile(QString urlSuffix) {
    QUrl getUrl = url + urlSuffix;
}

void NetworkManager::Delete(QString urlSuffix, QFile *toDelete) {
    QUrl deleteUrl = url + urlSuffix;
    QNetworkRequest request(deleteUrl); // replace URL w/ path to the file going to be deleted
    manager_.deleteResource(request);
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

void NetworkManager::readJson() {
    qDebug() << socket_->readAll();
    socket_->write("herro");
}

void NetworkManager::handleError(QAbstractSocket::SocketError socketError) {
    qDebug() << socketError;
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

void NetworkManager::onSignInManagerFinished(QNetworkReply *reply) {
    QByteArray buffer = reply->readAll();
    qDebug() << buffer;
    QJsonDocument jsonDoc(QJsonDocument::fromJson(buffer));
    QJsonObject jsonReply = jsonDoc.object();

    QJsonObject message = jsonReply["Message"].toObject();
    QJsonObject status = jsonReply["Status"].toObject();
    QJsonArray fileSystemObject = jsonReply["FileSystemObject"].toArray();
    QJsonObject userid = jsonReply["Userid"].toObject();
}

void NetworkManager::onSignUpManagerFinished(QNetworkReply *reply) {
    QByteArray buffer = reply->readAll();
    qDebug() << buffer;
    QJsonDocument jsonDoc(QJsonDocument::fromJson(buffer));
    QJsonObject jsonReply = jsonDoc.object();

    QJsonObject message = jsonReply["Message"].toObject();
}

void NetworkManager::onSignOutManagerFinished(QNetworkReply *reply) {
    QByteArray buffer = reply->readAll();
    qDebug() << buffer;
    QJsonDocument jsonDoc(QJsonDocument::fromJson(buffer));
    QJsonObject jsonReply = jsonDoc.object();

    QJsonObject message = jsonReply["Message"].toObject();
}
