#ifndef LOGINDIALOG_H
#define LOGINDIALOG_H

#include <QDialog>
#include <QDebug>
#include <QSysInfo>
#include "signupdialog.h"
#include "mainwindow.h"

namespace Ui {
class LoginDialog;
}

class LoginDialog : public QDialog
{
    Q_OBJECT

public:
    explicit LoginDialog(QWidget *parent = nullptr);
    ~LoginDialog();

private slots:
    void on_signInButton_clicked();

    void on_signInButton_2_clicked();

signals:
    void SetUsernameSignal(QString username);

    void SetPasswordSignal(QString password);

    void LoadSceneSignal();

private slots:
    void SignInSuccessful();

private:
    Ui::LoginDialog *ui;

    SignUpDialog* signup_;

};

#endif // LOGINDIALOG_H
