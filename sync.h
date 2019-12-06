#ifndef SYNC_H
#define SYNC_H

#include <QObject>
#include <QFileSystemWatcher>
#include <QJsonDocument>
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

public slots:
    virtual void HandleSync(QJsonDocument directoryJson) {}

signals:
    void CompareDirectory(QFileInfoList files);

protected:
    QFileSystemWatcher* watcher_;
};

#endif // SYNC_H
