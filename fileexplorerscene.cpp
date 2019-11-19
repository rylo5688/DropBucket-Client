#include "fileexplorerscene.h"

FileExplorerScene::FileExplorerScene()
{
//    QPixmap pixmap("://icons/icon_file.png");
//    addPixmap(pixmap);
    Q_INIT_RESOURCE(resources);

    curr_x_ = 0;
    curr_y_ = 0;

    factory_ = new SimpleDirectoryFactory();

    QJsonObject json = OpenReadJSON();
    CreateDirectoryComposite(json);
    LoadScene(root_dir_);
    curr_dir_ = root_dir_;
}

void FileExplorerScene::AddIcons(std::vector<Directory*> contents) {
    std::vector<Directory*>::iterator it;
    qDebug() << "Contents:";
    for(int i = 0; i < contents.size(); i++) {
        qDebug() << contents[i];
    }
    int offset_x = 50;
    int offset_y = 10;
    for(it = contents.begin(); it != contents.end(); it++){
        qDebug() << "gere" << *it;
        AddIcon(curr_x_ + offset_x, curr_y_ + offset_y, *it);
        curr_x_ += 56 + offset_x;
    }
}

void FileExplorerScene::AddIcon(int x, int y, Directory* toAdd) {
    toAdd->setPos(QPointF(x,y));
    addItem(toAdd);
    curr_loaded_.push_back(toAdd);
//    toAdd->setPos(QPointF(-500, 500));
}

void FileExplorerScene::LoadCurrDirParent() {
    Directory *toLoad = curr_dir_->getParent();
    if(toLoad != nullptr) {
        qDebug() << "Loading parent";
        LoadScene(toLoad);
    }
}

void FileExplorerScene::LoadScene(Directory* dir) {
//    clear();
    if(curr_loaded_.size() != 0) {
        std::vector<Directory*>::iterator it;
        for(it = curr_loaded_.begin(); it != curr_loaded_.end(); it++) {
            removeItem(*it);
        }
    }
    curr_loaded_.clear();

    curr_dir_ = dir;
    curr_x_ = 0;
    curr_y_ = 0;
    qDebug() << "cleared scene, loading contents";
    std::vector<Directory*> contents = dir->getContents();
    qDebug() << "Got contents";
    qDebug() << contents.at(0);
    AddIcons(dir->getContents());
    UpdateDirectoryLabel(QString::fromStdString(dir->getRelativePath()));
    update();
}

QJsonObject FileExplorerScene::OpenReadJSON() {
    Q_INIT_RESOURCE(resources);
    QFile loadFile(QStringLiteral("://jsons/test.json"));

    if(!loadFile.open(QIODevice::ReadOnly)) {
        qDebug() << "couldn't read JSON";
    }

    QByteArray dirData = loadFile.readAll();

    QJsonParseError err;
    QJsonDocument dirDoc(QJsonDocument::fromJson(dirData, &err));

    QJsonObject obj = dirDoc.object();

    return obj;
}

bool checkInVisited(std::vector<QJsonObject> *visited, QJsonObject find) {
    if(visited->size() == 0) {
        return false;
    }

    for(auto it = visited->begin(); it != visited->end(); it++) {
        if(*it == find) {
            return true;
        }
    }

    return false;
}

/**
 * @brief FileExplorerScene::CreateDirectoryComposite
 * This function updates the directoryMap_ to reflect the composite structure defined by
 * the json.
 * @param json Reference to the QJsonObject to create the composite structure from.
 */
void FileExplorerScene::CreateDirectoryComposite(QJsonObject &json) {
    std::map<QString, Directory*>::iterator mapIt;
    qDebug() << json;
    QJsonArray files = json["Files"].toArray();
    qDebug() << files;
    QJsonArray::iterator it;
    for(it = files.begin(); it != files.end(); it++) {
        QJsonObject currFile = (*it).toObject();
        QString name = currFile.keys()[0];
        currFile = currFile[name].toObject();
        QString md5 = currFile["md5"].toString();
        QString relativePath = currFile["relativePath"].toString();
        QStringList splitPath = relativePath.split("/", QString::SkipEmptyParts);
        qDebug() << splitPath;
        mapIt = directoryMap_.find(relativePath);
        Directory *newDir;
        if(mapIt != directoryMap_.end()) {
            // directory map contains the directory
            newDir = directoryMap_[relativePath];
        }
        else {
            //directory map doesn't contain the directory - create it
            newDir = factory_->createDir(0, 0, "folder", splitPath[splitPath.size() - 1].toStdString());
            newDir->setRelativePath(relativePath.toStdString());
            splitPath.pop_back();
            directoryMap_[relativePath] = newDir;

            if(relativePath == "/DropBucket/") {
                root_dir_ = newDir;
            }

            QString parent = "/" + splitPath.join('/') + "/";
            if(parent != "//") {
                qDebug() << parent;
                Folder* p = qgraphicsitem_cast<Folder*>(directoryMap_[parent]);
                p->AddDir(newDir);
                qDebug() << "updated";
                newDir->setParent(p);
            }
        }
        Directory *file = factory_->createDir(0, 0, "file", name.toStdString(), md5.toStdString());
        newDir->AddDir(file);
        file->setRelativePath(relativePath.toStdString());
    }

    qDebug() << directoryMap_.size();
}

void FileExplorerScene::mousePressEvent(QGraphicsSceneMouseEvent *event) {
    QGraphicsScene::mousePressEvent(event);
    if(itemAt(event->scenePos(), QTransform()) != nullptr) {
        QGraphicsItem *clicked = itemAt(event->scenePos(), QTransform());
        qDebug() << clicked->type();
        // TODO - handle clicks on folders = render new scene
        if(clicked->type() == 65538) {
            // Folder - open its contents
            if(clicked != root_dir_) {
                Folder* dir = qgraphicsitem_cast<Folder*>(clicked);
                if(clicked != 0) {
                    LoadScene(dir);
                }
                else {
                    qDebug() << "error";
                }
            }
        }
    }
}
