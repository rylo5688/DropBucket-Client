#include "logindialog.h"
#include "ui_logindialog.h"

#include <QOAuth2AuthorizationCodeFlow>
#include <QOAuthHttpServerReplyHandler>
#include <QDesktopServices>
#include <QString>
#include <QFile>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QNetworkReply>
#include <QDir>

LoginDialog::LoginDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::LoginDialog)
{
    ui->setupUi(this);
}

LoginDialog::~LoginDialog()
{
    delete ui;
}

void LoginDialog::on_signInButton_clicked()
{
    auto google = new QOAuth2AuthorizationCodeFlow;
    google->setScope("email");
    connect(google, &QOAuth2AuthorizationCodeFlow::authorizeWithBrowser, &QDesktopServices::openUrl);

    QString val;
//    QFile file("../DropBucket-Client/client_secret_345154858390-sflcog4b6ritr42hjc5rim0uakdo85ep.apps.googleusercontent.com.json");
    QFile file;
    // ../../../
    if (!QDir::setCurrent(QStringLiteral("../")))
        qDebug() << "Could not change the current working directory";

    file.setFileName("DropBucket-Client/client_secret.json");
    qDebug() << file.exists();

    if(file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        val = file.readAll();
        file.close();
        qDebug() << "opened successfully";
     } else {
        qDebug() << "failed to open";
    }

    qWarning() << val;
    QJsonDocument document = QJsonDocument::fromJson(val.toUtf8());

    const auto object = document.object();
    const auto settingsObject = object["web"].toObject();
    const QUrl authUri(settingsObject["auth_uri"].toString());
    const auto clientId = settingsObject["client_id"].toString();
    const QUrl tokenUri(settingsObject["token_uri"].toString());
    const auto clientSecret(settingsObject["client_secret"].toString());
    const auto redirectUris = settingsObject["redirect_uris"].toArray();
    const QUrl redirectUri(redirectUris[0].toString()); // Get the first URI
    const auto port = static_cast<quint16>(redirectUri.port()); // Get the port

    google->setAuthorizationUrl(authUri);
    google->setClientIdentifier(clientId);
    google->setAccessTokenUrl(tokenUri);
    google->setClientIdentifierSharedKey(clientSecret);

    auto replyHandler = new QOAuthHttpServerReplyHandler(port, this);
    google->setReplyHandler(replyHandler);
    google->grant();

    connect(google, &QOAuth2AuthorizationCodeFlow::granted, [=](){
       qDebug() << __FUNCTION__ << __LINE__ << "Access Granted!";
       auto reply = google->get(QUrl("https://www.googleapis.com/plus/v1/people/me"));
       connect(reply, &QNetworkReply::finished, [reply](){
           qDebug() << "REQUEST FINISHED. Error? " << (reply->error() != QNetworkReply::NoError);
           qDebug() << reply->readAll();
       });
    });
    // CHECK OPENSSL VERSION HERE
    qDebug() << QSslSocket::supportsSsl() << QSslSocket::sslLibraryBuildVersionString() << QSslSocket::sslLibraryVersionString();
    qDebug() << "sign in button clicked";
}
