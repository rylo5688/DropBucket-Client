#include "logindialog.h"
#include "ui_logindialog.h"
#include "networkmanager.h"

#include <QOAuth2AuthorizationCodeFlow>
#include "networkmanager.h"

/**
 * @brief LoginDialog::LoginDialog
 * Login Dialog contructor
 * @param parent Parent
 */
LoginDialog::LoginDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::LoginDialog)
{
    ui->setupUi(this);
    this->setWindowTitle("  ");
    connect(NetworkManager::getInstance(), &NetworkManager::SignInSuccessfulSignal, this, &LoginDialog::SignInSuccessful);
}

/**
 * @brief LoginDialog::~LoginDialog
 * Destroys LoginDialog
 */
LoginDialog::~LoginDialog()
{
    delete ui;
}

/**
 * @brief LoginDialog::on_signInButton_clicked
 * Handles when the sign in button is clicked
 */
void LoginDialog::on_signInButton_clicked()
{
    const QString username = ui->emailLineEdit->text();
    const QString password = ui->passwordLineEdit->text();

    QByteArray id = QSysInfo::machineUniqueId();
    QString data = QString("{\"username\":\"%1\",\"password\":\"%2\",\"device_id\":\"%3\"}").arg(username, password, id);
    QByteArray toPost = data.toUtf8();
    NetworkManager* nMgr = NetworkManager::getInstance();

    nMgr->SignInPost(&toPost);

    SetUsernameSignal(username);
    SetPasswordSignal(password);
    LoadSceneSignal();
}

/**
 * @brief LoginDialog::on_signInButton_2_clicked
 * Handles when the sign up button is clicked
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
    msgBox.setWindowTitle("   ");
    msgBox.setText("Sign in successful!");
    msgBox.exec();
    this->close();
}
