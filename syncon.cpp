#include "syncon.h"

SyncOn::SyncOn() : Sync()
{
    watcher_ = new QFileSystemWatcher;
    QObject::connect(watcher_, &QFileSystemWatcher::directoryChanged, this, &SyncOn::DirChanged);
    QObject::connect(watcher_, &QFileSystemWatcher::fileChanged, this, &SyncOn::FileChanged);
}

/**
  When directory changes, compare directory contents to what is in corresponding Directory
  (fileExplorerScene), send necessary request
  When file changes, send
 */

/**
 * @brief SyncOn::WatchDirectory
 * @param dir
 */
void SyncOn::WatchDirectory(QDir dir) {
    QDirIterator it(dir.absolutePath(), QDir::Dirs, QDirIterator::Subdirectories);
    QStringList toAdd;
    toAdd.append(dir.absolutePath());
    while (it.hasNext()) {
//        qDebug() << it.next();
        QString path = it.next();
        QStringList list = path.split("/", QString::SkipEmptyParts);
        qDebug() << list;
        if(!list.isEmpty() && (!list.last().compare(".") || !list.last().compare(".."))) {
            qDebug() << list.last();
//            watcher_->addPath(it.next());
        }
        else {
            qDebug() << "Here";
            toAdd.append(path);
        }
        qDebug() << "----";
    }
    watcher_->addPaths(toAdd);

    QDirIterator fileIt(dir.absolutePath(), QStringList() << "*", QDir::Files, QDirIterator::Subdirectories);
    while(fileIt.hasNext()) {
        watcher_->addPath(fileIt.next());
    }

    qDebug() << watcher_->files();
    qDebug() << watcher_->directories();
}

/**
 * @brief SyncOn::DirChanged
 * @param path
 */
void SyncOn::DirChanged(const QString &path) {
    qDebug() << "Directory changed: " << path;
    // Make into QDir
    // Get contents - need to compare it to what the fileExplorerScene has in map
    QDir dir(path);
    dir.setFilter(QDir::Files);
    qDebug() << dir.entryInfoList();
    CompareDirectory(dir.entryInfoList());
}

void SyncOn::FileChanged(const QString &path) {
    qDebug() << "File changed:" << path;
}

void SyncOn::HandleSync() {}

SyncOn::~SyncOn() {
    delete watcher_;
}
