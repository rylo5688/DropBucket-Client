#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QLabel>
#include "fileexplorerscene.h"
#include "logindialog.h"
#include "sync.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    void UISetUp();

private:
    Ui::MainWindow *ui;

    FileExplorerScene *fileExplorerScene;

    QLabel *filePathLabel;

    std::string currentDirectory;

    LoginDialog* login_;

    Sync* sync_;

private slots:
    void on_uploadButton_clicked();

    void on_profileButton_clicked();

    void on_syncButton_clicked();

    void on_returnButton_clicked();

    void UpdateDirectoryLabel(QString label);
};
#endif // MAINWINDOW_H
