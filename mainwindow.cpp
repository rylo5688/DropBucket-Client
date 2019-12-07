#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QDebug>

QString MainWindow::username = "";
QString MainWindow::password = "";
QString MainWindow::userid = "";

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    homeDir_ = QDir::homePath() + "/Dropbucket";
    UISetUp();
    deleteAct = new QAction(tr("&Delete"), this);
    connect(deleteAct, &QAction::triggered, this, &MainWindow::deleteFile);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::SetSyncButtonIcon(QString path) {
    QPushButton *syncButton = ui->syncButton;
    QPixmap pixmap(path);
    QIcon ButtonIcon(pixmap);
    syncButton->setIcon(ButtonIcon);
    syncButton->setIconSize(QSize(32,32));
}

void MainWindow::EnableWindow() {
    this->setEnabled(true);
}

void MainWindow::DisableWindow() {
    this->setEnabled(false);
    qDebug() << "Window disabled";
}

/**
 * Function to setup the folder structure.
 */
void MainWindow::UISetUp() {
    profile_ = new ProfileDialog(this);
    upload_ = new UploadDialog(this);
    fileExplorerScene = new FileExplorerScene;
    fileExplorerScene->setSceneRect(0,0,ui->fileGraphicsView->size().width(), ui->fileGraphicsView->size().height());
    ui->fileGraphicsView->setScene(fileExplorerScene);

    // Connect signals
    connect(fileExplorerScene, &FileExplorerScene::UpdateDirectoryLabel, this, &MainWindow::UpdateDirectoryLabel);
    connect(fileExplorerScene, &FileExplorerScene::FileAdded, this, &MainWindow::FileAdded);
    connect(fileExplorerScene, &FileExplorerScene::FileDeletedSignal, this, &MainWindow::FileDeleted);
    connect(upload_, &UploadDialog::UploadFile, fileExplorerScene, &FileExplorerScene::AddFile);

    connect(NetworkManager::getInstance(), &NetworkManager::SignInLoadScene, fileExplorerScene, &FileExplorerScene::SignInSuccess);
    connect(NetworkManager::getInstance(), &NetworkManager::SyncAndLoadScene, fileExplorerScene, &FileExplorerScene::Sync);

    filePathLabel = ui->pwdLabel;
    filePathLabel->setText("/DropBucket/"); // Place holder

    setWindowTitle(tr("DropBucket"));

    sync_ = new SyncOn();
    syncStatus_ = true;
    SetSyncButtonIcon(":/icons/icon_sync.png");

    connect(sync_, &SyncOn::CompareDirectory, fileExplorerScene, &FileExplorerScene::CompareDirectory);
    connect(fileExplorerScene, &FileExplorerScene::HandleSync, sync_, &Sync::HandleSync);
    connect(sync_, &SyncOn::DisableWindow, this, &MainWindow::DisableWindow);
    connect(sync_, &SyncOn::EnableWindow, this, &MainWindow::EnableWindow);
    connect(NetworkManager::getInstance(), &NetworkManager::DownloadCompleteSignal, sync_, &Sync::DownloadCompleted);
    connect(NetworkManager::getInstance(), &NetworkManager::SetUserid, this, &MainWindow::SetUserid);
}

/**
 * @brief MainWindow::SetState
 * @param state
 */
void MainWindow::SetState(Sync *state) {
    sync_ = state;
}

/**
 * @brief MainWindow::UpdateDirectoryLabel
 * @param label
 */
void MainWindow::UpdateDirectoryLabel(QString label) {
    filePathLabel->setText(label);
}

/**
 * @brief MainWindow::deleteFile
 */
void MainWindow::deleteFile() {
    qDebug() << "file";
}

void MainWindow::FileDeleted(QString relativePath) {
    QString filePath = homeDir_.path() + "/" + relativePath;
    sync_->RemovePath(filePath);
    QFile file(filePath);
    file.remove();
}

/**
 * @brief MainWindow::on_syncButton_clicked
 */
void MainWindow::on_syncButton_clicked() {
    qDebug() << "Sync button clicked";
    delete sync_;
    if(syncStatus_) {
        qDebug() << "sync off";
        sync_ = new SyncOff();
        SetSyncButtonIcon(":/icons/icon_nosync.png");
    }
    else {
        qDebug() << "sync on";
        sync_ = new SyncOn();
        SetSyncButtonIcon(":/icons/icon_sync.png");
        // Add directory paths
        qDebug() << fileExplorerScene->getDirectoryKeys();
        sync_->WatchDirectory(homeDir_);
    }
    syncStatus_ = !syncStatus_;
}

/**
 * @brief MainWindow::on_returnButton_clicked
 */
void MainWindow::on_returnButton_clicked() {
    qDebug() << "Return button clicked";
    fileExplorerScene->LoadCurrDirParent();
}

/**
 * @brief MainWindow::on_uploadButton_clicked
 */
void MainWindow::on_uploadButton_clicked() {
    // Start upload dialog
    qDebug() << "Upload button clicked";
    upload_->show();
}

/**
 * @brief MainWindow::on_profileButton_clicked
 */
void MainWindow::on_profileButton_clicked() {
    // Start profile dialog
    qDebug() << "Profile button clicked";
    profile_->show();
}

/**
 * @brief MainWindow::SetUserInfo
 * @param username
 * @param password
 */
void MainWindow::SetUsername(QString username) {
    this->username = username;
}

void MainWindow::SetPassword(QString password) {
    this->password = password;
}

void MainWindow::SetUserid(QString userid) {
    this->userid = userid;
}

/**
 * @brief MainWindow::SignInSuccessful
 */
void MainWindow::SignInSuccessful() {
//    fileExplorerScene->SignInSuccess();
}

void MainWindow::DownloadFinished() {
    sync_->WatchDirectory(homeDir_);
}

void MainWindow::FileAdded(QString filePath) {
    if(!sync_->CheckIfWatching(filePath)) {
        qDebug() << "Watching" << filePath;
        sync_->WatchFile(filePath);
    }
}
