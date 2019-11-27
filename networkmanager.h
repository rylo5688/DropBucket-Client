#ifndef NETWORKMANAGER_H
#define NETWORKMANAGER_H

#include <QObject>
#include <QTcpSocket>
#include <QDataStream>
#include <QNetworkAccessManager>

class NetworkManager : public QObject
{
    Q_OBJECT
public:
    NetworkManager();

private slots:
    void connected();
    void disconnected();
    void readJson();
    void handleError(QAbstractSocket::SocketError socketError);

private:
    QTcpSocket* socket_;
    QNetworkAccessManager manager_;
    QDataStream in;
};

#endif // NETWORKMANAGER_H
