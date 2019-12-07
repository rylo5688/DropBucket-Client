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
//    CompareDirectory(dir.entryInfoList());
}

void SyncOn::FileChanged(const QString &path) {
    qDebug() << "File changed:" << path;
    QStringList splitPath = path.split("/", QString::SkipEmptyParts);
    splitPath.pop_back();
    QString dropbucketDirPath = QDir::homePath() + "/Dropbucket";
//    qDebug() << splitPath.join("/").remove(0,dropbucketDirPath.size());
    NetworkManager::getInstance()->FilePost(path, splitPath.join("/").remove(0,dropbucketDirPath.size()));
}

void SyncOn::DisplaySyncingWindow() {
    window_ = new QWidget;
    window_->setWindowFlags(Qt::Window | Qt::WindowMinimizeButtonHint);
    window_->setGeometry(600,500,16,16);

    QPalette pal;
    pal.setColor(QPalette::Background, Qt::white);
    window_->setAutoFillBackground(true);
    window_->setPalette(pal);

    QHBoxLayout *layout = new QHBoxLayout();
    loadingGif_ = new QMovie();
    loadingGif_->setFileName("://icons/spinning-arrows.gif");
    QLabel *loadingLabel = new QLabel();
    QLabel *textLabel = new QLabel();
    textLabel->setText("Syncing please wait");
    loadingLabel->setMovie(loadingGif_);

    layout->addWidget(loadingLabel);
    layout->addWidget(textLabel);

    window_->setLayout(layout);
    window_->show();

    loadingGif_->start();
}

void SyncOn::CloseSyncingWindow() {
    loadingGif_->stop();
    window_->close();
}

void SyncOn::WatchFile(QString fileName) {
    qDebug() << watcher_->addPath(fileName);
}

bool SyncOn::CheckIfWatching(QString file) {
    return watcher_->directories().contains(file);
}

void SyncOn::HandleSync(QJsonArray directoriesArray, QJsonArray filesArray) {
    watcher_->removePaths(watcher_->files());
    DisableWindow();
    // Loading dialog
    DisplaySyncingWindow();

    QString dropbucketPath = QDir::homePath() + "/Dropbucket";
    QDir dropbucketDir(dropbucketPath);
    QDir homeDir(QDir::homePath());
    if(dropbucketDir.exists()) {
        // Delete the root dir recursively
        qDebug() << "Directory exists";
        qDebug() << dropbucketDir.removeRecursively();
    }

    // Set up the directories
    QJsonArray::iterator it;
    for(it = directoriesArray.begin(); it != directoriesArray.end(); it++) {
        QString relativePath = (*it).toString();
        QString directoryPath = dropbucketPath + "/" + relativePath;
        QDir temp(directoryPath);
        if(!temp.exists()) {
            qDebug() << homeDir.mkpath(directoryPath);
        }
    }

    // Download all the files
    QStringList downloadQueue;
    for(it = filesArray.begin(); it != filesArray.end(); it++) {
        QJsonObject fileObj = (*it).toObject();
        QString relativePath = fileObj.keys()[0];
        downloadQueue.append(relativePath);
    }
    NetworkManager::getInstance()->DownloadFiles(downloadQueue);
}

void SyncOn::RemovePath(QString path) {
    watcher_->removePath(path);
}

void SyncOn::DownloadCompleted() {
    qDebug() << "Download complete";
    EnableWindow();
    CloseSyncingWindow();
}

SyncOn::~SyncOn() {
    delete watcher_;
}
