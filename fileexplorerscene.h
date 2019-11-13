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

    void LoadScene(QString directory); // function to load all the icons of the current directory

    void mousePressEvent(QGraphicsSceneMouseEvent *event) override; // handles when mouse clicks a file/folder

//    void dragEnterEvent(QGraphicsSceneDragDropEvent *event) override;

//    void dropEvent(QGraphicsSceneDragDropEvent *event) override;

    void write(QJsonObject &json) const;

    void AddIcon(int x, int y);

    QJsonObject OpenReadJSON();

    std::vector<std::pair<QJsonObject, std::vector<QJsonObject>>> GetFileStructureVector(QJsonObject &json);

    void CreateDirectoryComposite(QJsonObject &json);

private:

    QJsonObject* currJson_;
    SimpleDirectoryFactory* factory_;
    std::vector<Directory*> directories_;

    int curr_x_;
    int curr_y_; // both for plotting

    Directory *root_dir_;
    Directory *curr_dir_;
};

#endif // FILEEXPLORERSCENE_H
