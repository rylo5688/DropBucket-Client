#ifndef SYNCON_H
#define SYNCON_H

#include <QObject>
#include <QDebug>
#include <QDir>
#include <QDirIterator>
#include "sync.h"
//#include <QMainWindow>
//#include "mainwindow.h"

class SyncOn : public Sync
{
    Q_OBJECT
public:
    SyncOn();

    void WatchDirectory(QDir dir) override;

    void WatchFile(QString fileName);

    ~SyncOn() override;

public slots:
    void HandleSync(QJsonDocument directoryJson) override;

private slots:
    void DirChanged(const QString &path);

    void FileChanged(const QString &path);
};

#endif // SYNCON_H
