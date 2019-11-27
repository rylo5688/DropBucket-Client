#ifndef SYNCON_H
#define SYNCON_H

#include <QObject>
#include <QDebug>
#include <QDir>
#include "sync.h"
//#include <QMainWindow>
//#include "mainwindow.h"

class SyncOn : public Sync
{
    Q_OBJECT
public:
    SyncOn();

    void WatchDirectory(QString dir) override;

    void WatchDirectories(QStringList dirsToAdd) override;

    void HandleSync() override;

    ~SyncOn() override;

private slots:
    void DirChanged(const QString &path);
};

#endif // SYNCON_H
