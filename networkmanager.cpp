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

    in.setDevice(socket_);
    in.setVersion(QDataStream::Qt_5_0);
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
