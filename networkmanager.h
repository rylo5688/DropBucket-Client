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
#include <QHttpMultiPart>
#include <QHttpPart>
#include <QRandomGenerator>

class NetworkManager : public QObject
{
    Q_OBJECT
public:

    void SignInPost(QByteArray *toPost);

    void SignUpPost(QByteArray *toPost);

    void SignOutPost(QByteArray *toPost);

    bool FilePost(QString filePath, QString DirectoryAddedTo);

    void FileGet(QString filePath);

    void FileDelete(QString relativePath);

    void DownloadFiles(QStringList relativePaths);

    static NetworkManager* getInstance();

signals:
    void SignUpSuccessful();

    void SignInSuccessful();

    void SetUserid(QString userid);

    void SignInLoadScene(QJsonArray directoriesArray, QJsonArray filesArray);

    void SyncAndLoadScene(QJsonArray directoriesArray, QJsonArray filesArray);

    void DownloadCompleteSignal();

    void FileDeleteSuccessful();

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

    void onGetFileReadyRead();

    void onFileManagerFinished(QNetworkReply *reply);

    void onDeleteFileManagerFinished(QNetworkReply *reply);

    void downloadProgress(qint64 bytesReceived, qint64 bytesTotal);

    void DownloadComplete(QNetworkReply *reply);

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
//    QVector<QString> downloadQueue_;
    QVector<QNetworkReply*> currentDownloads_;
};

#endif // NETWORKMANAGER_H
