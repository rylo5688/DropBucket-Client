#ifndef SYNC_H
#define SYNC_H

#include <QObject>
#include <QFileSystemWatcher>
#include <QFileInfoList>
#include <QDir>
//#include <QMainWindow>
//#include "mainwindow.h"

class Sync : public QObject
{
    Q_OBJECT
public:
    Sync();

    virtual void WatchDirectory(QDir dir) {}

    virtual void HandleSync() {}

signals:
    void CompareDirectory(QFileInfoList files);

protected:
    QFileSystemWatcher* watcher_;
};

#endif // SYNC_H
