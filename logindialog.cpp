#include "logindialog.h"
#include "ui_logindialog.h"

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
    // PUT OAUTH STUFF HERE
    qDebug() << "sign in button clicked";
}
