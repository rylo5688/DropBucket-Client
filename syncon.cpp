#include "syncon.h"

/**
 * @brief SyncOn::SyncOn
 * Sync on constructor
 */
SyncOn::SyncOn() : Sync()
{
    watcher_ = new QFileSystemWatcher;
    QObject::connect(watcher_, &QFileSystemWatcher::directoryChanged, this, &SyncOn::DirChanged);
    QObject::connect(watcher_, &QFileSystemWatcher::fileChanged, this, &SyncOn::FileChanged);
}

/**
 * @brief SyncOn::WatchDirectory
 * Watches a directory by recursively going down and add the files to the watcher
 * @param dir Directory to watch
 */
void SyncOn::WatchDirectory(QDir dir) {
    QDirIterator it(dir.absolutePath(), QDir::Dirs, QDirIterator::Subdirectories);

    QDirIterator fileIt(dir.absolutePath(), QStringList() << "*", QDir::Files, QDirIterator::Subdirectories);
    while(fileIt.hasNext()) {
        watcher_->addPath(fileIt.next());
    }
}

/**
 * @brief SyncOn::DirChanged
 * Handles when a directory is changed - currently not used but planned to be
 * @param path Path to the directory
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

/**
 * @brief SyncOn::FileChanged
 * Handles when a file is changed
 * @param path Path to the changed file
 */
void SyncOn::FileChanged(const QString &path) {
    QStringList splitPath = path.split("/", QString::SkipEmptyParts);
    splitPath.pop_back();
    QString dropbucketDirPath = QDir::homePath() + "/Dropbucket";
    NetworkManager::getInstance()->FilePost(path, splitPath.join("/").remove(0,dropbucketDirPath.size()));
}

/**
 * @brief SyncOn::DisplaySyncingWindow
 * Displays the syncing window
 */
void SyncOn::DisplaySyncingWindow() {
    window_ = new QWidget;
    window_->setWindowTitle("    ");
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

/**
 * @brief SyncOn::CloseSyncingWindow
 * Closes the syncing window and stops the loading gif
 */
void SyncOn::CloseSyncingWindow() {
    loadingGif_->stop();
    window_->close();
}

/**
 * @brief SyncOn::WatchFile
 * Watches a file
 * @param fileName file path to watch
 */
void SyncOn::WatchFile(QString fileName) {
    qDebug() << watcher_->addPath(fileName);
}

/**
 * @brief SyncOn::CheckIfWatching
 * Checks to see if a file is being watched
 * @param file File to check
 * @return Whether it is being watched or not
 */
bool SyncOn::CheckIfWatching(QString file) {
    return watcher_->directories().contains(file);
}

/**
 * @brief SyncOn::HandleSync
 * Handles a sync
 * @param directoriesArray Directory's on the bucket
 * @param filesArray Files in the bucket
 */
void SyncOn::HandleSync(QJsonArray directoriesArray, QJsonArray filesArray) {
    qDebug() << "Handling sync";
    watcher_->removePaths(watcher_->files());
    DisableWindowSignal();
    // Loading dialog
    DisplaySyncingWindow();

    QString dropbucketPath = QDir::homePath() + "/Dropbucket";
    QDir dropbucketDir(dropbucketPath);
    QDir homeDir(QDir::homePath());
    if(dropbucketDir.exists()) {
        // Delete the root dir recursively
        qDebug() << dropbucketDir.removeRecursively();
    }

    homeDir.mkdir("Dropbucket");
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

    if(!downloadQueue.empty()) {
        NetworkManager::getInstance()->DownloadFiles(downloadQueue);
    }
    else{
        EnableWindowSignal();
        CloseSyncingWindow();
    }
}

/**
 * @brief SyncOn::RemovePath
 * Removes a path from the watcher
 * @param path Path to remove
 */
void SyncOn::RemovePath(QString path) {
    watcher_->removePath(path);
}

/**
 * @brief SyncOn::DownloadCompleted
 * Handles when the sync downloads are finished, closes window
 */
void SyncOn::DownloadCompleted() {
    qDebug() << "Download complete";
    EnableWindowSignal();
    CloseSyncingWindow();
}

/**
 * @brief SyncOn::~SyncOn
 * Sync on destructor
 */
SyncOn::~SyncOn() {
    delete watcher_;
}
