#ifndef NETWORKMANAGER_H
#define NETWORKMANAGER_H

#include <QObject>
#include <QTcpSocket>
#include <QDataStream>
#include <QNetworkReply>
#include <QVariant>
#include <QNetworkAccessManager>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QFile>

class NetworkManager : public QObject
{
    Q_OBJECT
public:

    void SignInPost(QByteArray *toPost);

    void SignUpPost(QByteArray *toPost);

    void SignOutPost(QByteArray *toPost);

    void GetFile(QString urlSuffix);

    void Delete(QString urlSuffix, QFile* toDelete);

    static NetworkManager* getInstance();

private slots:
    void connected();
    void disconnected();
    void readJson();
    void handleError(QAbstractSocket::SocketError socketError);

    void onManagerFinished(QNetworkReply *reply);

    void onSignInManagerFinished(QNetworkReply *reply);

    void onSignUpManagerFinished(QNetworkReply *reply);

    void onSignOutManagerFinished(QNetworkReply *reply);

private:
    // C++ Singleton: https://gist.github.com/pazdera/1098119
    NetworkManager();

    static NetworkManager* instance_;

    void Post(QNetworkAccessManager *manager, QNetworkRequest *request, QByteArray *toPost);

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
