#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QDebug>

QString MainWindow::username = "";
QString MainWindow::password = "";
QString MainWindow::userid = "";

/**
 * @brief MainWindow::MainWindow
 * Main Window constructor
 * @param parent Parent
 */
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    this->setWindowTitle("  ");
    homeDir_ = QDir::homePath() + "/Dropbucket";
    UISetUp();
    deleteAct_ = new QAction(tr("&Delete"), this);
    connect(deleteAct_, &QAction::triggered, this, &MainWindow::DeleteFile);
}

/**
 * @brief MainWindow::~MainWindow
 * Main Window destructor
 */
MainWindow::~MainWindow()
{
    delete ui;
}

/**
 * @brief MainWindow::SetSyncButtonIcon
 * Sets the sync button to the sync icon corresponding to current sync state
 * @param path
 */
void MainWindow::SetSyncButtonIcon(QString path) {
    QPushButton *syncButton = ui->syncButton;
    QPixmap pixmap(path);
    QIcon ButtonIcon(pixmap);
    syncButton->setIcon(ButtonIcon);
    syncButton->setIconSize(QSize(32,32));
}

/**
 * @brief MainWindow::EnableWindow
 * Enables the main window
 */
void MainWindow::EnableWindow() {
    this->setEnabled(true);
}

/**
 * @brief MainWindow::DisableWindow
 * Disables the main window
 */
void MainWindow::DisableWindow() {
    this->setEnabled(false);
    qDebug() << "Window disabled";
}

/**
 * @brief MainWindow::UISetUp
 * Sets up the Main Window's UI
 */
void MainWindow::UISetUp() {
    profile_ = new ProfileDialog(this);
    upload_ = new UploadDialog(this);
    fileExplorerScene_ = new FileExplorerScene;
    fileExplorerScene_->setSceneRect(0,0,ui->fileGraphicsView->size().width(), ui->fileGraphicsView->size().height());
    ui->fileGraphicsView->setScene(fileExplorerScene_);

    // Connect signals
    connect(fileExplorerScene_, &FileExplorerScene::UpdateDirectoryLabelSignal, this, &MainWindow::UpdateDirectoryLabel);
    connect(fileExplorerScene_, &FileExplorerScene::FileAddedSignal, this, &MainWindow::FileAdded);
    connect(fileExplorerScene_, &FileExplorerScene::FileDeletedSignal, this, &MainWindow::FileDeleted);
    connect(upload_, &UploadDialog::UploadFileSignal, fileExplorerScene_, &FileExplorerScene::AddFile);

    connect(NetworkManager::getInstance(), &NetworkManager::SignInLoadSceneSignal, fileExplorerScene_, &FileExplorerScene::SignInSuccess);
    connect(NetworkManager::getInstance(), &NetworkManager::SyncAndLoadSceneSignal, fileExplorerScene_, &FileExplorerScene::Sync);

    filePathLabel_ = ui->pwdLabel;
    filePathLabel_->setText("/DropBucket/"); // Place holder

//    setWindowTitle(tr("DropBucket"));

    sync_ = new SyncOn();
    syncStatus_ = true;
    SetSyncButtonIcon(":/icons/icon_sync.png");

    connect(fileExplorerScene_, &FileExplorerScene::HandleSyncSignal, sync_, &Sync::HandleSync);
    connect(sync_, &SyncOn::DisableWindowSignal, this, &MainWindow::DisableWindow);
    connect(sync_, &SyncOn::EnableWindowSignal, this, &MainWindow::EnableWindow);
    connect(NetworkManager::getInstance(), &NetworkManager::DownloadCompleteSignal, sync_, &Sync::DownloadCompleted);
    connect(NetworkManager::getInstance(), &NetworkManager::SetUseridSignal, this, &MainWindow::SetUserid);
}

/**
 * @brief MainWindow::SetState
 * Sets the Sync state
 * @param state Sync state
 */
void MainWindow::SetState(Sync *state) {
    sync_ = state;
}

/**
 * @brief MainWindow::UpdateDirectoryLabel
 * Updates the directory's label to the current relative file path
 * @param label
 */
void MainWindow::UpdateDirectoryLabel(QString label) {
    filePathLabel_->setText(label);
}

/**
 * @brief MainWindow::deleteFile
 * Handles when a file is deleted
 */
void MainWindow::DeleteFile() {
    // Nothing to do yet
}

/**
 * @brief MainWindow::FileDeleted
 * Slot for when a file is deleted
 * @param relativePath
 */
void MainWindow::FileDeleted(QString relativePath) {
    QString filePath = homeDir_.path() + "/" + relativePath;
    sync_->RemovePath(filePath);
    QFile file(filePath);
    file.remove();
}

/**
 * @brief MainWindow::on_syncButton_clicked
 * Handles when the sync button is clicked, set the opposite state
 */
void MainWindow::on_syncButton_clicked() {
    delete sync_;
    if(syncStatus_) {
        sync_ = new SyncOff();
        SetSyncButtonIcon(":/icons/icon_nosync.png");
    }
    else {
        sync_ = new SyncOn();
        SetSyncButtonIcon(":/icons/icon_sync.png");
        // Add directory paths
        qDebug() << fileExplorerScene_->getDirectoryKeys();
        sync_->WatchDirectory(homeDir_);
    }
    syncStatus_ = !syncStatus_;
}

/**
 * @brief MainWindow::on_returnButton_clicked
 * Handles when the return button is clicked, load the parent directory
 */
void MainWindow::on_returnButton_clicked() {
    fileExplorerScene_->LoadCurrDirParent();
}

/**
 * @brief MainWindow::on_uploadButton_clicked
 * Handles when the upload button is clicked, shows upload dialog
 */
void MainWindow::on_uploadButton_clicked() {
    // Start upload dialog
    upload_->show();
}

/**
 * @brief MainWindow::on_profileButton_clicked
 * Handles when profile button is clicked, shows Profile dialog
 */
void MainWindow::on_profileButton_clicked() {
    // Start profile dialog
    profile_->show();
}

/**
 * @brief MainWindow::SetUsername
 * Sets the users username
 * @param username Users username
 */
void MainWindow::SetUsername(QString username) {
    this->username = username;
}

/**
 * @brief MainWindow::SetPassword
 * Sets the users password
 * @param password Users password
 */
void MainWindow::SetPassword(QString password) {
    this->password = password;
}

/**
 * @brief MainWindow::SetUserid
 * Sets the user id
 * @param userid Users userid
 */
void MainWindow::SetUserid(QString userid) {
    this->userid = userid;
}

/**
 * @brief MainWindow::SignInSuccessful
 * Slot for when the sign in is successful
 */
void MainWindow::SignInSuccessful() {
//    fileExplorerScene->SignInSuccess();
}

/**
 * @brief MainWindow::DownloadFinished
 * Slot for when the sync downloads are completed
 */
void MainWindow::DownloadFinished() {
    sync_->WatchDirectory(homeDir_);
}

/**
 * @brief MainWindow::FileAdded
 * Handles when a file is added, adds it to the watcher
 * @param filePath
 */
void MainWindow::FileAdded(QString filePath) {
    if(!sync_->CheckIfWatching(filePath)) {
        sync_->WatchFile(filePath);
    }
}
