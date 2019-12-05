#ifndef PROFILEDIALOG_H
#define PROFILEDIALOG_H

#include <QDialog>
#include <QDebug>
#include "networkmanager.h"
#include "mainwindow.h"

namespace Ui {
class ProfileDialog;
}

class ProfileDialog : public QDialog
{
    Q_OBJECT

public:
    explicit ProfileDialog(QWidget *parent = nullptr);
    ~ProfileDialog();

private slots:
    void on_signOutButton_clicked();

private:
    Ui::ProfileDialog *ui;
};

#endif // PROFILEDIALOG_H
