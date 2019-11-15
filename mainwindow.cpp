#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QDebug>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    login_ = new LoginDialog(this);
    UISetUp();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void SetupButtonIcon(QPushButton *button, std::string path) {
    qDebug() << "Setting up button icon";
    QPixmap pixmap(QString::fromStdString(path));
    QIcon ButtonIcon(pixmap);
    button->setIcon(ButtonIcon);
    button->setIconSize(pixmap.size());
}

/**
 * Function to setup the folder structure.
 */
void MainWindow::UISetUp() {

    fileExplorerScene = new FileExplorerScene;
    fileExplorerScene->setSceneRect(0,0,ui->fileGraphicsView->size().width(), ui->fileGraphicsView->size().height());
    ui->fileGraphicsView->setScene(fileExplorerScene);

    // Connect signals
    connect(fileExplorerScene, &FileExplorerScene::UpdateDirectoryLabel, this, &MainWindow::UpdateDirectoryLabel);

    filePathLabel = ui->pwdLabel;
    filePathLabel->setText("/DropBucket/"); // Place holder
//    QPushButton *syncButton = ui->syncButton;

//    SetupButtonIcon(syncButton, ":/icons/icon_sync.png");
    qDebug() << ui->fileGraphicsView->size();
}

void MainWindow::UpdateDirectoryLabel(QString label) {
    filePathLabel->setText(label);
}

void MainWindow::on_syncButton_clicked() {
    qDebug() << "Sync button clicked";
    // TODO - Sync State implemented
    // Want to change the state based on click

    // Update the icon to reflect the current state
}

void MainWindow::on_returnButton_clicked() {
    qDebug() << "Return button clicked";
    fileExplorerScene->LoadCurrDirParent();
}

void MainWindow::on_uploadButton_clicked() {
    // Start upload dialog
    qDebug() << "Upload button clicked";
}

void MainWindow::on_profileButton_clicked() {
    // Start profile dialog
    qDebug() << "Profile button clicked";
    login_->show();
}

