#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QDebug>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    UISetUp();
    deleteAct = new QAction(tr("&Delete"), this);
    connect(deleteAct, &QAction::triggered, this, &MainWindow::deleteFile);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::SetSyncButtonIcon(QString path) {
    qDebug() << "Setting up button icon";
    QPushButton *syncButton = ui->syncButton;
    QPixmap pixmap(path);
    QIcon ButtonIcon(pixmap);
    syncButton->setIcon(ButtonIcon);
    syncButton->setIconSize(QSize(32,32));
}

/**
 * Function to setup the folder structure.
 */
void MainWindow::UISetUp() {
    login_ = new LoginDialog(this);
    upload_ = new UploadDialog(this);
    fileExplorerScene = new FileExplorerScene;
    fileExplorerScene->setSceneRect(0,0,ui->fileGraphicsView->size().width(), ui->fileGraphicsView->size().height());
    ui->fileGraphicsView->setScene(fileExplorerScene);

    // Connect signals
    connect(fileExplorerScene, &FileExplorerScene::UpdateDirectoryLabel, this, &MainWindow::UpdateDirectoryLabel);
    connect(upload_, &UploadDialog::UploadFile, fileExplorerScene, &FileExplorerScene::AddFile);

    filePathLabel = ui->pwdLabel;
    filePathLabel->setText("/DropBucket/"); // Place holder

    setWindowTitle(tr("DropBucket"));

    sync_ = new SyncOn();
    syncStatus_ = true;
    SetSyncButtonIcon(":/icons/icon_sync.png");
    sync_->WatchDirectory("F:\\School\\4thYearSem1\\CSCI 4448 - OOAD\\DropBucket\\Test-Dir");
    connect(sync_, &SyncOn::CompareDirectory, fileExplorerScene, &FileExplorerScene::CompareDirectory);
    qDebug() << ui->fileGraphicsView->size();
}

void MainWindow::SetState(Sync *state) {
    sync_ = state;
}

void MainWindow::UpdateDirectoryLabel(QString label) {
    filePathLabel->setText(label);
}

void MainWindow::deleteFile() {
    qDebug() << "file";
}

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
        sync_->WatchDirectory("F:\\School\\4thYearSem1\\CSCI 4448 - OOAD\\DropBucket\\Test-Dir");
    }
    syncStatus_ = !syncStatus_;
}

void MainWindow::on_returnButton_clicked() {
    qDebug() << "Return button clicked";
    fileExplorerScene->LoadCurrDirParent();
}

void MainWindow::on_uploadButton_clicked() {
    // Start upload dialog
    qDebug() << "Upload button clicked";
    upload_->show();
}

void MainWindow::on_profileButton_clicked() {
    // Start profile dialog
    qDebug() << "Profile button clicked";
    login_->show();
}

