#include "fileexplorerscene.h"

/**
 * @brief FileExplorerScene::FileExplorerScene
 */
FileExplorerScene::FileExplorerScene()
{
    Q_INIT_RESOURCE(resources);

    curr_x_ = 0;
    curr_y_ = 0;

    factory_ = new SimpleDirectoryFactory();


    DirectoryToJson();
}

/**
 * @brief FileExplorerScene::SignInSuccess
 * TODO
 * Replace with JSON from HTTP response, FileSystemObject
 */
void FileExplorerScene::SignInSuccess(QJsonArray directoriesArray, QJsonArray filesArray) {
    qDebug() << "In FES sign in success slot";
    qDebug() << directoriesArray;
    qDebug() << filesArray;

//    QJsonObject json = OpenReadJSON();
    CreateDirectoryComposite(directoriesArray, filesArray);
//    QJsonDocument currDirStatus = DirectoryToJson();
//    QJsonArray files = currDirStatus["fs_objects"].toArray();
//    qDebug() << files;
    HandleSync(DirectoryToJson());
    LoadScene(root_dir_);
    curr_dir_ = root_dir_;

    qDebug() << "Testing Get::::";
    NetworkManager::getInstance()->FileGet("");
}

/**
 * @brief FileExplorerScene::DirectoryToJson
 * @return
 */
QJsonDocument FileExplorerScene::DirectoryToJson() {
    qDebug() << "-----------Directory to JSON------------";
    QString dropbucketDirPath = QDir::homePath() + "/Dropbucket/";
    QDir dropbucketDir(dropbucketDirPath);
    QDirIterator it(dropbucketDirPath, QStringList() << "*", QDir::Files, QDirIterator::Subdirectories);
    QJsonArray array;
    while(it.hasNext()) {
        QJsonObject fileObj;
        QJsonObject fileInfoObj;
        QString filePath = it.next();
        QFileInfo fi(filePath);
        QFile file(filePath);
        QString md5;
        if(file.open(QFile::ReadOnly)) {
            md5 = QCryptographicHash::hash(file.readAll(), QCryptographicHash::Md5).toHex();
        }
        QString relativePath = filePath;
        relativePath.remove(0, dropbucketDirPath.length());
        fileObj.insert(relativePath, md5);
        array.push_back(fileObj);
    }
    QJsonObject fileSystemObject;
    fileSystemObject.insert("fs_objects", array);
    QJsonDocument doc(fileSystemObject);
    qDebug() << doc;
    qDebug() << "----------------------------------------";

    return doc;
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

    update();
}

/**
 * @brief FileExplorerScene::LoadCurrDirParent
 */
void FileExplorerScene::LoadCurrDirParent() {
    if(curr_dir_ != root_dir_) {
        Directory *toLoad = curr_dir_->getParent();
        if(toLoad != nullptr) {
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
    std::vector<Directory*> contents = dir->getContents();
    AddIcons(dir->getContents());
    UpdateDirectoryLabel(dir->getRelativePath());
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
 * @param directoriesArray
 * @param filesArray
 */
void FileExplorerScene::CreateDirectoryComposite(QJsonArray directoriesArray, QJsonArray filesArray) {
    QJsonArray::iterator it;
    // First create all the directories, root dir first
    Directory *root = factory_->createDir(0, 0, "folder", "/");
    root->setRelativePath("/");
    directoryMap_["/"] = root;
    root_dir_ = root;
    for(it = directoriesArray.begin(); it != directoriesArray.end(); it++) {
        qDebug() << *it;
        QString relativePath = (*it).toString();
        QStringList splitPath = relativePath.split("/", QString::SkipEmptyParts);
        qDebug() << relativePath;
        qDebug() << splitPath;
        Directory *newDir = factory_->createDir(0, 0, "folder", splitPath[splitPath.size()- 1]);
        newDir->setRelativePath(relativePath);
        directoryMap_[relativePath] = newDir;
        splitPath.pop_back();
        QString parent = splitPath.join('/') + '/';
        qDebug() << parent;
        Folder* p = qgraphicsitem_cast<Folder*>(directoryMap_[parent]);
        p->AddDir(newDir);
        newDir->setParent(p);
    }

    // Directories created - fill them files
    for(it = filesArray.begin(); it != filesArray.end(); it++) {
        qDebug() << *it;
        QJsonObject fileObj = (*it).toObject();
        QString relativePath = fileObj.keys()[0];
        QStringList splitPath = relativePath.split("/", QString::SkipEmptyParts);
        QString fileName = splitPath[splitPath.length() - 1];
        QString parent;
        Directory *file = factory_->createDir(0, 0, "file", fileName, fileObj[relativePath].toString());
        file->setRelativePath(relativePath);
        if(splitPath.size() == 1) {
            // File is in root directory
            parent = "/";
        }
        else {
            splitPath.pop_back();
            parent = splitPath.join('/') + '/';
        }
        Folder* p = qgraphicsitem_cast<Folder*>(directoryMap_[parent]);
        p->AddDir(file);
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
            QString Md5 = md5.toHex();
            QString url = (*it).toLocalFile();
            AddFile(url, Md5);
            pathList.append(url);
        }
        qDebug() << pathList;

        // Now we can open and do stuff with the files - want to add file to scene
    }
}

void FileExplorerScene::AddFile(QString filePath, QString md5) {
    QStringList splitPath = filePath.split("/", QString::SkipEmptyParts);
    const QDir pathDir(filePath);
    if(!pathDir.exists()) {
        // Check it is a directory - maybe explore the whole folder and upload it - later
        qDebug() << "file";
        // Create the file - add it to the directory
        Directory* file = factory_->createDir(0, 0, "file", splitPath[splitPath.length()-1], md5);
        file->setRelativePath(curr_dir_->getRelativePath() + "/" + splitPath[splitPath.length()-1]);
        curr_dir_->AddDir(file);
        curr_x_ += 50;
        AddIcon(curr_x_, curr_y_ + 10, file);
        curr_x_ += 56;
        NetworkManager::getInstance()->FilePost(filePath, curr_dir_->getRelativePath());
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
