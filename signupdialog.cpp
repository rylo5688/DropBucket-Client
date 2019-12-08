#include "signupdialog.h"
#include "ui_signupdialog.h"

/**
 * @brief SignUpDialog::SignUpDialog
 * Sign up dialog constructor
 * @param parent Parent of the dialog
 */
SignUpDialog::SignUpDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::SignUpDialog)
{
    ui->setupUi(this);
    this->setWindowTitle("  ");
    connect(NetworkManager::getInstance(), &NetworkManager::SignUpSuccessfulSignal, this, &SignUpDialog::SignUpSuccessful);
}

/**
 * @brief SignUpDialog::~SignUpDialog
 * Sign up dialog constructor
 */
SignUpDialog::~SignUpDialog()
{
    delete ui;
}

/**
 * @brief SignUpDialog::on_signUpButton_clicked
 * Handles when sign up button is clicked
 */
void SignUpDialog::on_signUpButton_clicked()
{
    const QString username = ui->emailLineEdit->text();
    const QString password = ui->passwordLineEdit->text();
    const QString confirmPassword = ui->passwordLineEdit_2->text();

    if(password == confirmPassword) {
        QString data = QString("{\"username\":\"%1\",\"password\":\"%2\"}").arg(username, password);
        qDebug() << data;
        QByteArray toPost = data.toUtf8();
        NetworkManager *nMgr = NetworkManager::getInstance();

        nMgr->SignUpPost(&toPost);
    }
    else {
        QMessageBox msgBox;
        msgBox.setText("Passwords do not match.");
        msgBox.exec();
    }
}

/**
 * @brief SignUpDialog::SignUpSuccessful
 * Slot for if the sign up is successful
 */
void SignUpDialog::SignUpSuccessful() {
    QMessageBox msgBox;
    msgBox.setText("Sign up successful! Please Sign in.");
    msgBox.exec();

    this->close();
}
