#include "fileexplorerscene.h"

FileExplorerScene::FileExplorerScene()
{
//    QPixmap pixmap("://icons/icon_file.png");
//    addPixmap(pixmap);
    Q_INIT_RESOURCE(resources);
    LoadScene(QString::fromStdString("src"));
    factory_ = new SimpleDirectoryFactory();

    QJsonObject json = OpenReadJSON();
    CreateDirectoryComposite(json);
    LoadScene("/DropBucket/");
}

void FileExplorerScene::AddIcon(Directory* toAdd) {
    toAdd->setPos(QPointF(0,0));
    addItem(toAdd);
//    toAdd->setPos(QPointF(-500, 500));
}

QStringList GetKeys(QJsonArray &jsonArray, std::string type) {
    QJsonArray::iterator it;
    QStringList temp;
    for(it = jsonArray.begin(); it != jsonArray.end(); it++) {
        if(type == "folder") {
            temp = it->toObject().keys();
        }
        else if(type == "file") {
            temp.append(it->toObject().value("name").toString());
        }
    }

    return temp;
}

void FileExplorerScene::LoadScene(QString directory) {
    std::map<QString, Directory*>::iterator it;
    it = directoryMap_.find(directory);
    if(it != directoryMap_.end()) {
        // key exists load the scene
        Directory* toLoad = directoryMap_[directory];
        qDebug() << QString::fromStdString(toLoad->getContents()[0]->getName());
        AddIcon(toLoad->getContents()[2]);
    }
}

QJsonObject FileExplorerScene::OpenReadJSON() {
    Q_INIT_RESOURCE(resources);
    QFile loadFile(QStringLiteral("://jsons/test.json"));

    if(!loadFile.open(QIODevice::ReadOnly)) {
        qDebug() << "couldn't read JSON";
    }

    QByteArray dirData = loadFile.readAll();

//    qDebug() << dirData;
    QJsonParseError err;
    QJsonDocument dirDoc(QJsonDocument::fromJson(dirData, &err));
//    qDebug() << err.errorString();

    QJsonObject obj = dirDoc.object();

//    qDebug() << obj.size();

    return obj;
}

bool checkInVisited(std::vector<QJsonObject> *visited, QJsonObject find) {
    if(visited->size() == 0) {
        return false;
    }

    for(auto it = visited->begin(); it != visited->end(); it++) {
        if(*it == find) {
            qDebug() << "here";
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
            splitPath.pop_back();
            directoryMap_[relativePath] = newDir;

            QString parent = "/" + splitPath.join('/') + "/";
            if(parent != "//") {
                qDebug() << parent;
                Folder* p = qgraphicsitem_cast<Folder*>(directoryMap_[parent]);
                p->AddDir(newDir);
                qDebug() << "updated";
            }
        }
        Directory *file = factory_->createDir(1000, 100, "file", name.toStdString(), md5.toStdString());
        newDir->AddDir(file);
        file->setPos(QPointF(200,200));
    }

    qDebug() << directoryMap_.size();
}

void FileExplorerScene::mousePressEvent(QGraphicsSceneMouseEvent *event) {
    QGraphicsScene::mousePressEvent(event);
    if(itemAt(event->scenePos(), QTransform()) != nullptr) {
        QGraphicsItem *clicked = itemAt(event->scenePos(), QTransform());
        qDebug() << clicked;
    }
}
