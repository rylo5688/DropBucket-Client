#ifndef SYNCON_H
#define SYNCON_H

#include <QObject>
#include <QDebug>
#include <QDir>
#include <QDirIterator>
#include <QLabel>
#include <QMovie>
#include <QHBoxLayout>
#include <QPalette>
#include "sync.h"
#include "networkmanager.h"

class SyncOn : public Sync
{
    Q_OBJECT
public:
    SyncOn();

    void WatchDirectory(QDir dir) override;

    void WatchFile(QString fileName) override;

    void DisplaySyncingWindow();

    void CloseSyncingWindow();

    bool CheckIfWatching(QString file) override;

    void RemovePath(QString path) override;

    ~SyncOn() override;

public slots:
    void HandleSync(QJsonArray directoriesArray, QJsonArray filesArray) override;

    void DownloadCompleted() override;

private slots:
    void DirChanged(const QString &path);

    void FileChanged(const QString &path);

private:
    QWidget *window_;
    QMovie *loadingGif_;
};

#endif // SYNCON_H
