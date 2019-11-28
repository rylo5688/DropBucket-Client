#include "networkmanager.h"

/**
 * @brief NetworkManager::NetworkManager
 * TCP example - https://www.bogotobogo.com/cplusplus/sockets_server_client_QT.php
 * HTTP example - https://code.qt.io/cgit/qt/qtbase.git/tree/examples/network/http?h=5.13
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

    connect(&manager_, &QNetworkAccessManager::finished, this, &NetworkManager::onManagerFinished);

//    in.setDevice(socket_);
//    in.setVersion(QDataStream::Qt_5_0);
}

void NetworkManager::Put(QFile *toPut) {
    QNetworkRequest request(url); // replace with put url
    if(toPut->open(QIODevice::ReadOnly)) {
        QByteArray ba = toPut->readAll();
        manager_.put(request, ba);
    }
}

void NetworkManager::Post(QFile *toPost) {
    QNetworkRequest request(url); // replace w/ post url
    if(toPost->open(QIODevice::ReadOnly)) {
        QByteArray ba = toPost->readAll();
        manager_.post(request, ba);
    }
}

void NetworkManager::Get() {

}

void NetworkManager::Delete(QFile *toDelete) {
    QNetworkRequest request(url); // replace URL w/ path to the file going to be deleted
    manager_.deleteResource(request);
}

void NetworkManager::connected() {
    qDebug() << "connceted";
}

void NetworkManager::disconnected() {
    qDebug() << "disconnected";
}

void NetworkManager::readJson() {
    qDebug() << socket_->readAll();
    socket_->write("herro");
//    in.startTransaction();
//    QString temp;
//    in >> temp;
//    qDebug() << temp;
//    if(!in.commitTransaction()) {
//        qDebug() << "Commit failed";
//        return;
//    }
//    qDebug() << temp;
//    socket_->connectToHost("localhost", 12000);
}

void NetworkManager::handleError(QAbstractSocket::SocketError socketError) {
    qDebug() << socketError;
}

void NetworkManager::onManagerFinished(QNetworkReply *reply) {
    qDebug() << reply;
    QVariant status_code = reply->attribute(QNetworkRequest::HttpStatusCodeAttribute);
        status_code.is_valid(){
            // Print or catch the status code
            QString status = status_code.toString(); // or status_code.toInt();
            qDebug() << status;
        }
}
