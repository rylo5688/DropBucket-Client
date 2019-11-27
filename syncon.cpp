#include "syncon.h"

SyncOn::SyncOn() : Sync()
{
    watcher_ = new QFileSystemWatcher;
    QObject::connect(watcher_, &QFileSystemWatcher::directoryChanged, this, &SyncOn::DirChanged);
}

void SyncOn::WatchDirectory(QString dir) {
    qDebug() << watcher_->addPath(dir);
}

void SyncOn::WatchDirectories(QStringList dirsToAdd) {
    watcher_->addPaths(dirsToAdd);
}

void SyncOn::DirChanged(const QString &path) {
    qDebug() << path;
    // Make into QDir
    // Get contents - need to compare it to what the fileExplorerScene has in map
    QDir dir(path);
    dir.setFilter(QDir::Files);
    CompareDirectory(dir.entryInfoList());
}

void SyncOn::HandleSync() {}

SyncOn::~SyncOn() {
    delete watcher_;
}
