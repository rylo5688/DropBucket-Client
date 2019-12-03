#ifndef NETWORKMANAGER_H
#define NETWORKMANAGER_H

#include <QObject>
#include <QTcpSocket>
#include <QDataStream>
#include <QNetworkReply>
#include <QVariant>
#include <QNetworkAccessManager>
#include <QFile>

class NetworkManager : public QObject
{
    Q_OBJECT
public:
    void Put(QString urlSuffix, QFile *toPut);

    void Post(QString urlSuffix, QByteArray *toPost);

    void Get(QString urlSuffix);

    void Delete(QString urlSuffix, QFile* toDelete);

    static NetworkManager* getInstance();

private slots:
    void connected();
    void disconnected();
    void readJson();
    void handleError(QAbstractSocket::SocketError socketError);

    void onManagerFinished(QNetworkReply *reply);

private:
    // Singleton: https://gist.github.com/pazdera/1098119
    NetworkManager();

    static NetworkManager* instance_;

    QTcpSocket* socket_;
    QString url;
    QNetworkAccessManager manager_;
//    QNetworkRequest request_;
    QNetworkReply *reply_;
    QFile *file;
    bool httpRequestAborted_;
    QDataStream in;
};

#endif // NETWORKMANAGER_H
