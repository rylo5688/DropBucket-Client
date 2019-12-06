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
#include <QDir>
#include <QFile>
#include <QSysInfo>
#include <QImageReader>
#include <QBuffer>

class NetworkManager : public QObject
{
    Q_OBJECT
public:

    void SignInPost(QByteArray *toPost);

    void SignUpPost(QByteArray *toPost);

    void SignOutPost(QByteArray *toPost);

    void FilePost(QString filePath, QString DirectoryAddedTo);

    void FileGet(QString filePath);

    void FileDelete(QString filePath);

    static NetworkManager* getInstance();

signals:
    void SignUpSuccessful();

    void SignInSuccessful();

    void SetUserid(QString userid);

    void LoadScene(QJsonArray directoriesArray, QJsonArray filesArray);

private slots:
    void connected();
    void disconnected();
    void readJson();
    void handleError(QAbstractSocket::SocketError socketError);

    void onManagerFinished(QNetworkReply *reply);

    void onSignInManagerFinished(QNetworkReply *reply);

    void onSignUpManagerFinished(QNetworkReply *reply);

    void onSignOutManagerFinished(QNetworkReply *reply);

    void onGetFileManagerFinished(QNetworkReply *reply);

    void onFileManagerFinished(QNetworkReply *reply);

private:
    // C++ Singleton: https://gist.github.com/pazdera/1098119
    NetworkManager();

    static NetworkManager* instance_;

    void Post(QNetworkRequest *request, QByteArray *toPost);

    QTcpSocket* socket_;
    QString url;
    QNetworkAccessManager manager_;
//    QNetworkRequest request_;
    QNetworkReply *reply_;
    QFile *file;
    bool httpRequestAborted_;
    QString userid_;
};

#endif // NETWORKMANAGER_H
