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
    a.setWindowIcon(QIcon(":/icons/dropbucket.png"));
//    a.setApplicationDisplayName("");
    MainWindow w;
    w.setFixedSize(1280,720);
    LoginDialog ld;

    QObject::connect(&ld, &LoginDialog::SetUsername, &w, &MainWindow::SetUsername);
    QObject::connect(&ld, &LoginDialog::SetPassword, &w, &MainWindow::SetPassword);
    QObject::connect(&ld, &LoginDialog::LoadScene, &w, &MainWindow::SignInSuccessful);
    QObject::connect(NetworkManager::getInstance(), &NetworkManager::DownloadCompleteSignal, &w, &MainWindow::DownloadFinished);

    w.show();
    ld.show();

    return a.exec();
}


