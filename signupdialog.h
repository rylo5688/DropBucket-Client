#ifndef SIGNUPDIALOG_H
#define SIGNUPDIALOG_H

#include <QDialog>
#include <QDebug>
#include <QMessageBox>
#include "networkmanager.h"

namespace Ui {
class SignUpDialog;
}

class SignUpDialog : public QDialog
{
    Q_OBJECT

public:
    explicit SignUpDialog(QWidget *parent = nullptr);
    ~SignUpDialog();

private slots:
    void on_signUpButton_clicked();

    void SignUpSuccessful();

private:
    Ui::SignUpDialog *ui;
};

#endif // SIGNUPDIALOG_H
