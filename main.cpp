#include "mainwindow.h"

#include <QApplication>
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
    qDebug() << QDir::homePath();
    NetworkManager *n = NetworkManager::getInstance(); // init for testing
    w.show();
    return a.exec();
}
