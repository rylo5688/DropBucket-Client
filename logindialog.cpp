#include "logindialog.h"
#include "ui_logindialog.h"
#include "networkmanager.h"

#include <QOAuth2AuthorizationCodeFlow>
#include "networkmanager.h"

/**
 * @brief LoginDialog::LoginDialog
 * @param parent
 */
LoginDialog::LoginDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::LoginDialog)
{
    ui->setupUi(this);
    connect(NetworkManager::getInstance(), &NetworkManager::SignInSuccessful, this, &LoginDialog::SignInSuccessful);
}

/**
 * @brief LoginDialog::~LoginDialog
 */
LoginDialog::~LoginDialog()
{
    delete ui;
}

/**
 * @brief LoginDialog::on_signInButton_clicked
 */
void LoginDialog::on_signInButton_clicked()
{
    const QString username = ui->emailLineEdit->text();
    const QString password = ui->passwordLineEdit->text();

    qDebug() << "sign in button clicked";
    qDebug() << username;
    qDebug() << password;

    QByteArray id = QSysInfo::machineUniqueId();
    QString data = QString("{\"username\":\"%1\",\"password\":\"%2\",\"device_id\":\"%3\"}").arg(username, password, id);
    qDebug() << data;
    QByteArray toPost = data.toUtf8();
    NetworkManager* nMgr = NetworkManager::getInstance();

    nMgr->SignInPost(&toPost);

    SetUserInfo(username, password);
    LoadScene();
}

/**
 * @brief LoginDialog::on_signInButton_2_clicked
 */
void LoginDialog::on_signInButton_2_clicked()
{
    signup_ = new SignUpDialog;
    signup_->show();
}

/**
 * @brief LoginDialog::SignInSuccessful
 * Notifies the user the sign in was a success, closes dialog.
 */
void LoginDialog::SignInSuccessful() {
    QMessageBox msgBox;
    msgBox.setText("Sign in successful!");
    msgBox.exec();
    this->close();
}
