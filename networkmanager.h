#ifndef NETWORKMANAGER_H
#define NETWORKMANAGER_H

#include <QObject>
#include <QTcpSocket>
#include <QDataStream>
#include <QNetworkAccessManager>
#include <QFile>

class NetworkManager : public QObject
{
    Q_OBJECT
public:
    NetworkManager();

    void Put(QFile *toPut);

    void Post(QFile *toPost);

    void Get();

    void Delete(QFile* toDelete);

private slots:
    void connected();
    void disconnected();
    void readJson();
    void handleError(QAbstractSocket::SocketError socketError);

    void onManagerFinished(QNetworkReply *reply);

private:
    QTcpSocket* socket_;
    QUrl url;
    QNetworkAccessManager manager_;
//    QNetworkRequest request_;
    QNetworkReply *reply_;
    QFile *file;
    bool httpRequestAborted_;
    QDataStream in;
};

#endif // NETWORKMANAGER_H
