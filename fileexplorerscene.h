#ifndef FILEEXPLORERSCENE_H
#define FILEEXPLORERSCENE_H

#include <QMenu>
#include <QObject>
#include <QGraphicsScene>
#include <QMimeData>
#include <QWidget>
#include <QFile>
#include <QDebug>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QJsonValue>
#include <QUrl>
#include <QDir>
#include <QGraphicsSceneMouseEvent>
#include <QStandardPaths>
#include <QCryptographicHash>
#include <QDirIterator>
#include <queue>
#include "directory.h"
#include "datafile.h"
#include "simpledirectoryfactory.h"
#include "networkmanager.h"

// JSON example: https://doc.qt.io/qt-5/qtcore-serialization-savegame-example.html
class FileExplorerScene : public QGraphicsScene
{
    Q_OBJECT
public:
    FileExplorerScene();

    void LoadCurrDirParent();

    void LoadScene(Directory* dir); // function to load all the icons of the current directory

    QString GetMimeType(const QMimeData *inData);

    void mousePressEvent(QGraphicsSceneMouseEvent *event) override; // handles when mouse clicks a file/folder

    void dragEnterEvent(QGraphicsSceneDragDropEvent *event) override;

    void dragMoveEvent(QGraphicsSceneDragDropEvent *event) override;

    void dropEvent(QGraphicsSceneDragDropEvent *event) override;

    void write(QJsonObject &json) const;

    void AddIcon(int x, int y, Directory* toAdd);

    void AddIcons(std::vector<Directory*> contents);

    void CreateDirectoryComposite(QJsonArray directoriesArray, QJsonArray filesArray);

    int getDirectoryMapSize() { return directoryMap_.size(); }

    QStringList getDirectoryKeys();

    QJsonDocument DirectoryToJson();

    void ClearComposite();

signals:
    void UpdateDirectoryLabelSignal(QString label);

    void HandleSyncSignal(QJsonArray directoriesArray, QJsonArray filesArray);

    void FileAddedSignal(QString path);

    void FileDeletedSignal(QString relativePath);

public slots:
    void AddFile(QString filePath, QString Md5);

    void SignInSuccess(QJsonArray directoriesArray, QJsonArray filesArray);

    void DisableScene();

    void EnableScene();

//    void LoadRootDir();

    void Sync(QJsonArray directoriesArray, QJsonArray filesArray);

private slots:
    void FileDeleted(Directory *deleted);

private:

    bool allowClicks_;

    QJsonObject* currJson_;
    SimpleDirectoryFactory* factory_;
    std::map<QString, Directory*> directoryMap_;

    int curr_x_;
    int curr_y_; // both for plotting

    Directory *root_dir_;
    Directory *curr_dir_;
    std::vector<Directory*> curr_loaded_;
};

#endif // FILEEXPLORERSCENE_H
