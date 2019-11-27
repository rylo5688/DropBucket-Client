#ifndef SYNC_H
#define SYNC_H

#include <QObject>
#include <QFileSystemWatcher>
#include <QFileInfoList>
//#include <QMainWindow>
//#include "mainwindow.h"

class Sync : public QObject
{
    Q_OBJECT
public:
    Sync();

    virtual void WatchDirectory(QString dir) {}

    virtual void WatchDirectories(QStringList dirsToAdd) {}

    virtual void HandleSync() {}

signals:
    void CompareDirectory(QFileInfoList files);

protected:
    QFileSystemWatcher* watcher_;
};

#endif // SYNC_H
