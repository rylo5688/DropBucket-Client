#include "fileexplorerscene.h"

/**
 * @brief FileExplorerScene::FileExplorerScene
 */
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

    /**
    TODO
    - Delete file menu - ask team
    - Profile Dialog
    - Start log in dialog on start up
    */

}

/**
 * @brief FileExplorerScene::AddIcons
 * @param contents
 */
void FileExplorerScene::AddIcons(std::vector<Directory*> contents) {
    std::vector<Directory*>::iterator it;
    int offset_x = 50;
    int offset_y = 10;
    for(it = contents.begin(); it != contents.end(); it++){
        AddIcon(curr_x_ + offset_x, curr_y_ + offset_y, *it);
        curr_x_ += 56 + offset_x;
    }
}

/**
 * @brief FileExplorerScene::AddIcon
 * @param x
 * @param y
 * @param toAdd
 */
void FileExplorerScene::AddIcon(int x, int y, Directory* toAdd) {
    toAdd->setPos(QPointF(x,y));
    addItem(toAdd);
    curr_loaded_.push_back(toAdd);

    qDebug() << curr_x_ << curr_y_;
    update();
}

/**
 * @brief FileExplorerScene::LoadCurrDirParent
 */
void FileExplorerScene::LoadCurrDirParent() {
    if(curr_dir_ != root_dir_) {
        Directory *toLoad = curr_dir_->getParent();
        if(toLoad != nullptr) {
            qDebug() << "Loading parent";
            LoadScene(toLoad);
        }
    }
}

/**
 * @brief FileExplorerScene::LoadScene
 * @param dir
 */
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
    qDebug() << "cleared scen e, loading contents";
    std::vector<Directory*> contents = dir->getContents();
    qDebug() << "Got contents";
    qDebug() << contents.at(0);
    AddIcons(dir->getContents());
    UpdateDirectoryLabel(QString::fromStdString(dir->getRelativePath()));
    update();
}

/**
 * @brief FileExplorerScene::OpenReadJSON
 * @return
 */
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

/**
 * @brief checkInVisited
 * @param visited
 * @param find
 * @return
 */
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
    QJsonArray files = json["FileSystemObject"].toArray();
    QJsonArray::iterator it;
    for(it = files.begin(); it != files.end(); it++) {
        QJsonObject currFile = (*it).toObject();
        QString name = currFile.keys()[0];
        currFile = currFile[name].toObject();
        QString md5 = currFile["md5"].toString();
        QString relativePath = currFile["relativePath"].toString();
        QStringList splitPath = relativePath.split("/", QString::SkipEmptyParts);
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
                Folder* p = qgraphicsitem_cast<Folder*>(directoryMap_[parent]);
                p->AddDir(newDir);
                newDir->setParent(p);
            }
        }
        Directory *file = factory_->createDir(0, 0, "file", name.toStdString(), md5.toStdString());
        newDir->AddDir(file);
        file->setRelativePath(relativePath.toStdString());
    }
}

/**
 * @brief FileExplorerScene::dragMoveEvent
 * @param event
 */
void FileExplorerScene::dragMoveEvent(QGraphicsSceneDragDropEvent *event) {
    event->acceptProposedAction();
}

/**
 * @brief FileExplorerScene::dragEnterEvent
 * @param event
 */
void FileExplorerScene::dragEnterEvent(QGraphicsSceneDragDropEvent *event) {
    event->acceptProposedAction();
}

// https://wiki.qt.io/Drag_and_Drop_of_files
/**
 * @brief FileExplorerScene::dropEvent
 * @param event
 */
void FileExplorerScene::dropEvent(QGraphicsSceneDragDropEvent *event) {
    const QMimeData* mimeData = event->mimeData();

    if(mimeData->hasUrls()) {
        QStringList pathList;
        QList<QUrl> urlLIst = mimeData->urls();

        QList<QUrl>::iterator it;
        for(it = urlLIst.begin(); it != urlLIst.end(); it++) {
            QByteArray md5 = QCryptographicHash::hash(mimeData->data(GetMimeType(mimeData)), QCryptographicHash::Md5);
            std::string Md5 = QString(md5).toStdString();
            QString url = (*it).toLocalFile();
            AddFile(url, Md5);
            pathList.append(url);
        }
        qDebug() << pathList;

        // Now we can open and do stuff with the files - want to add file to scene
    }
}

void FileExplorerScene::AddFile(QString filePath, std::string md5) {
    QStringList splitPath = filePath.split("/", QString::SkipEmptyParts);
    const QDir pathDir(filePath);
    if(!pathDir.exists()) {
        // Check it is a directory - maybe explore the whole folder and upload it - later
        qDebug() << "file";
        // Create the file - add it to the directory
        Directory* file = factory_->createDir(0, 0, "file", splitPath[splitPath.length()-1].toStdString(), md5);
        file->setRelativePath(curr_dir_->getRelativePath() + "/" + splitPath[splitPath.length()-1].toStdString());
        curr_dir_->AddDir(file);
        curr_x_ += 50;
        AddIcon(curr_x_, curr_y_ + 10, file);
        curr_x_ += 56;
    }
}

void FileExplorerScene::CompareDirectory(QFileInfoList files) {
    qDebug() << "COMPARING";
    qDebug() << files;
    // Want to use the most updated file info
}

/**
 * @brief FileExplorerScene::GetMimeType
 * @param inData
 * @return
 */
QString FileExplorerScene::GetMimeType(const QMimeData *inData) {
    if(inData->hasHtml()){
        return "text/html";
    }
    else if(inData->hasText()) {
        return "text/plain";
    }
    else if(inData->hasImage()) {
        return "image/*";
    }
    else {
        return "no";
    }
}

/**
 * @brief FileExplorerScene::getDirectoryKeys
 * @return
 */
QStringList FileExplorerScene::getDirectoryKeys() {
    QStringList directories;
    std::map<QString, Directory*>::iterator it;
    for(it = directoryMap_.begin(); it != directoryMap_.end(); it++) {
        directories.push_back(it->first);
    }

    return directories;
}

/**
 * @brief FileExplorerScene::mousePressEvent
 * @param event
 */
void FileExplorerScene::mousePressEvent(QGraphicsSceneMouseEvent *event) {
    QGraphicsScene::mousePressEvent(event);
    if(itemAt(event->scenePos(), QTransform()) != nullptr) {
        QGraphicsItem *clicked = itemAt(event->scenePos(), QTransform());
        if(event->button() == Qt::LeftButton) {
            qDebug() << clicked->type();
            if(clicked->type() == 65538) {
                // Folder - open its contents
                if(clicked != root_dir_) {
                    Folder* dir = qgraphicsitem_cast<Folder*>(clicked);
                    if(clicked != nullptr) {
                        LoadScene(dir);
                    }
                    else {
                        qDebug() << "error";
                    }
                }
            }
        }
        else if(event->button() == Qt::RightButton) {
            if(clicked->type() == 65539) {
                qDebug() << "File!";
            }
        }

    }
}
