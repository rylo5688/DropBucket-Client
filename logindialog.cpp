#include "logindialog.h"
#include "ui_logindialog.h"
#include "networkmanager.h"

#include <QOAuth2AuthorizationCodeFlow>

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

    NetworkManager* nMgr = new NetworkManager;
    nMgr->post("http://localhost:8080/users/", data.toUtf8()); // Needs to be changed to https
}
