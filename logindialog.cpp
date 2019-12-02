#include "logindialog.h"
#include "ui_logindialog.h"
#include "networkmanager.h"

#include <QOAuth2AuthorizationCodeFlow>
#include "networkmanager.h"

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
    // PUT LOGIN STUFF HERE
    const QString username = ui->emailLineEdit->text();
    const QString password = ui->passwordLineEdit->text();

    qDebug() << "sign in button clicked";
    qDebug() << username;
    qDebug() << password;

    QString data = QString("{\"username\":\"%1\",\"password\":\"%2\"}").arg(username, password);
    QFile file;
    file.write(data.toUtf8());
    NetworkManager* nMgr = new NetworkManager;

    nMgr->Post(&file);
}
