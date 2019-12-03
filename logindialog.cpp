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

    QByteArray id = QSysInfo::machineUniqueId();
    QString data = QString("{\"Username\":\"%1\",\"Password\":\"%2\",\"Deviceid\":\"%3\"}").arg(username, password, id);
//    QFile file;
    qDebug() << data;
    QByteArray toPost = data.toUtf8();
    NetworkManager* nMgr = NetworkManager::getInstance();

    nMgr->Post("/users/signin", &toPost);
}

void LoginDialog::on_signInButton_2_clicked()
{
    qDebug() << "sign up clicked";
    signup_ = new SignUpDialog;
    signup_->show();
    this->close();
}
