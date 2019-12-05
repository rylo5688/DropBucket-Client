#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QLabel>
#include <QMenu>
#include <QContextMenuEvent>
#include "fileexplorerscene.h"
#include "logindialog.h"
#include "profiledialog.h"
#include "sync.h"
#include "syncon.h"
#include "syncoff.h"
#include "uploaddialog.h"

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

    void SetSyncButtonIcon(QString path);

    static QString username;

    static QString password;

private:

    void SetState(Sync* state);

    QDir homeDir_;

    Ui::MainWindow *ui;

    FileExplorerScene *fileExplorerScene;

    QLabel *filePathLabel;

    std::string currentDirectory;

    ProfileDialog* profile_;

    UploadDialog* upload_;

    Sync* sync_;

    bool syncStatus_;

    QAction *deleteAct;

private slots:
    void on_uploadButton_clicked();

    void on_profileButton_clicked();

    void on_syncButton_clicked();

    void on_returnButton_clicked();

    void UpdateDirectoryLabel(QString label);

    void deleteFile();

public slots:
    void SetUserInfo(QString username, QString password);

    void SignInSuccessful();
};
#endif // MAINWINDOW_H
