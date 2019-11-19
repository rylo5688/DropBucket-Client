#ifndef FILEEXPLORERSCENE_H
#define FILEEXPLORERSCENE_H

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
#include <QGraphicsSceneMouseEvent>
#include <queue>
#include "directory.h"
#include "datafile.h"
#include "simpledirectoryfactory.h"

class FileExplorerScene : public QGraphicsScene
{
    Q_OBJECT
public:
    FileExplorerScene();

    void LoadCurrDirParent();

    void LoadScene(Directory* dir); // function to load all the icons of the current directory

    void mousePressEvent(QGraphicsSceneMouseEvent *event) override; // handles when mouse clicks a file/folder

//    void dragEnterEvent(QGraphicsSceneDragDropEvent *event) override;

//    void dropEvent(QGraphicsSceneDragDropEvent *event) override;

    void write(QJsonObject &json) const;

    void AddIcon(int x, int y, Directory* toAdd);

    void AddIcons(std::vector<Directory*> contents);

    QJsonObject OpenReadJSON();

    void CreateDirectoryComposite(QJsonObject &json);

    int getDirectoryMapSize() { return directoryMap_.size(); }

signals:
    void UpdateDirectoryLabel(QString label);

private:

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
