#include "signupdialog.h"
#include "ui_signupdialog.h"

SignUpDialog::SignUpDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::SignUpDialog)
{
    ui->setupUi(this);
    connect(NetworkManager::getInstance(), &NetworkManager::SignUpSuccessful, this, &SignUpDialog::SignUpSuccessful);
}

SignUpDialog::~SignUpDialog()
{
    delete ui;
}

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

void SignUpDialog::SignUpSuccessful() {
    QMessageBox msgBox;
    msgBox.setText("Sign up successful! Please Sign in.");
    msgBox.exec();

    this->close();
}
