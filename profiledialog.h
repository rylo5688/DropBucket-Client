#ifndef PROFILEDIALOG_H
#define PROFILEDIALOG_H

#include <QDialog>
#include <QDebug>
#include "networkmanager.h"

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


private:
    Ui::ProfileDialog *ui;
};

#endif // PROFILEDIALOG_H
