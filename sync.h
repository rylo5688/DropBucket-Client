#ifndef SYNC_H
#define SYNC_H

#include <QObject>
#include <QFileSystemWatcher>
#include <QJsonDocument>
#include <QJsonArray>
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

    virtual void WatchFile(QString filePath) {}

    virtual bool CheckIfWatching(QString file) { return false; }

    virtual void RemovePath(QString path) {}

public slots:
    virtual void HandleSync(QJsonArray directoriesArray, QJsonArray filesArray) {}

    virtual void DownloadCompleted() {}

signals:
    void CompareDirectory(QFileInfoList files);

    void DisableWindow();

    void EnableWindow();

protected:
    QFileSystemWatcher* watcher_;
};

#endif // SYNC_H
