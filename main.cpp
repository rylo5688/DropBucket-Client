#include "mainwindow.h"

#include <QApplication>
#include <QObject>
#include "logindialog.h"
#include "networkmanager.h"

/**
 * @brief qMain
 * @param argc
 * @param argv
 * @return
 *
 * Root directory location:
 * - HomePath//DropBucket
 */
int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    MainWindow w;
    LoginDialog ld;

    QObject::connect(&ld, &LoginDialog::SetUserInfo, &w, &MainWindow::SetUserInfo);
    QObject::connect(&ld, &LoginDialog::LoadScene, &w, &MainWindow::SignInSuccessful);

    w.show();
    ld.show();

    return a.exec();
}


