#include "fileexplorerscene.h"

/**
 * @brief FileExplorerScene::FileExplorerScene
 * Constructor for File Explorer
 */
FileExplorerScene::FileExplorerScene()
{
    Q_INIT_RESOURCE(resources);

    allowClicks_ = true;

    curr_x_ = 0;
    curr_y_ = 0;

    factory_ = new SimpleDirectoryFactory();

    DirectoryToJson();
}

/**
 * @brief FileExplorerScene::SignInSuccess
 * Handles the sign in process when the sign in is successful
 * @param directoriesArray Directory array from server response
 * @param filesArray File array from server resposne
 */
void FileExplorerScene::SignInSuccess(QJsonArray directoriesArray, QJsonArray filesArray) {
    CreateDirectoryComposite(directoriesArray, filesArray);
    qDebug() << "Composite created";

    HandleSyncSignal(directoriesArray, filesArray);
    LoadScene(root_dir_);
    curr_dir_ = root_dir_;
}

/**
 * @brief FileExplorerScene::EnableScene
 * Allows for interaction with the FileExplorerScene
 */
void FileExplorerScene::EnableScene() {
    allowClicks_ = true;
}

/**
 * @brief FileExplorerScene::DisableScene
 * Disallows for interaction with the FileExplorerScene
 */
void FileExplorerScene::DisableScene() {
    allowClicks_ = false;
}

/**
 * @brief FileExplorerScene::DirectoryToJson
 * Reads the Dropbucket directory and makes a JSON reflective of it. CURRENT UNUSED.
 * @return QJsonDocument of the current directory status
 */
QJsonDocument FileExplorerScene::DirectoryToJson() {
    // TODO: Add "directories"
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

    return doc;
}

/**
 * @brief FileExplorerScene::AddIcons
 * Adds icons to the scene
 * @param contents Contents of the directory to add the icons of
 */
void FileExplorerScene::AddIcons(std::vector<Directory*> contents) {
    std::vector<Directory*>::iterator it;
    int offset_x = 50;
    int offset_y = 10;
    for(it = contents.begin(); it != contents.end(); it++){
        AddIcon(curr_x_ + offset_x, curr_y_ + offset_y, *it);
        curr_x_ += 56 + offset_x;
        if(curr_x_ > 1200) {
            curr_x_ = 0;
            curr_y_ += 100;
        }
    }
}

/**
 * @brief FileExplorerScene::AddIcon
 * Adds a icon to the scene
 * @param x X location
 * @param y Y location
 * @param toAdd Directory to add
 */
void FileExplorerScene::AddIcon(int x, int y, Directory* toAdd) {
    qDebug() << "adding icon";
    toAdd->setPos(QPointF(x,y));
    addItem(toAdd);
    curr_loaded_.push_back(toAdd);

    update();
}

/**
 * @brief FileExplorerScene::LoadCurrDirParent
 * Loads the current directories parent
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
 * Loads a directory to the scene
 * @param dir Directory to load
 */
void FileExplorerScene::LoadScene(Directory* dir) {
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
    UpdateDirectoryLabelSignal(dir->getRelativePath());
    update();
}

/**
 * @brief FileExplorerScene::CreateDirectoryComposite
 * Creates the Directory composite of the Bucket's current file structure
 * @param directoriesArray Array of the directories on Bucket
 * @param filesArray Array of the files in Bucket
 */
void FileExplorerScene::CreateDirectoryComposite(QJsonArray directoriesArray, QJsonArray filesArray) {
    qDebug() << "Creating composite";
    QJsonArray::iterator it;
    // First create all the directories, root dir first
    Directory *root = factory_->createDir(0, 0, "folder", "/");
    root->setRelativePath("/");
    directoryMap_["/"] = root;
    root_dir_ = root;
    for(it = directoriesArray.begin(); it != directoriesArray.end(); it++) {
        QString relativePath = (*it).toString();
        QStringList splitPath = relativePath.split("/", QString::SkipEmptyParts);
        Directory *newDir = factory_->createDir(0, 0, "folder", splitPath[splitPath.size()- 1]);
        newDir->setRelativePath(relativePath);
        directoryMap_[relativePath] = newDir;
        splitPath.pop_back();
        QString parent = splitPath.join('/') + '/';
        Folder* p = qgraphicsitem_cast<Folder*>(directoryMap_[parent]);
        p->AddDir(newDir);
        newDir->setParent(p);
    }

    // Directories created - fill them files
    for(it = filesArray.begin(); it != filesArray.end(); it++) {
        QJsonObject fileObj = (*it).toObject();
        QString relativePath = fileObj.keys()[0];
        QStringList splitPath = relativePath.split("/", QString::SkipEmptyParts);
        QString fileName = splitPath[splitPath.length() - 1];
        QString parent;
        Directory *file = factory_->createDir(0, 0, "file", fileName, fileObj[relativePath].toString());
        connect(file, &DataFile::Deleted, this, &FileExplorerScene::FileDeleted);
        connect(NetworkManager::getInstance(), &NetworkManager::FileDeleteSuccessfulSignal, file, &Directory::NetworkDeleteSuccessful);
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
        file->setParent(p);
        p->AddDir(file);
    }
}

/**
 * @brief FileExplorerScene::FileDeleted
 * Removes the deleted file from the FileExplorerScene
 * @param deleted The deleted File
 */
void FileExplorerScene::FileDeleted(Directory *deleted) {
    FileDeletedSignal(deleted->getRelativePath());
    removeItem(deleted);
    LoadScene(curr_dir_);
    update();
}

/**
 * @brief FileExplorerScene::ClearComposite
 * Clears the Composite structure (Could have memory leak, not sure)
 */
void FileExplorerScene::ClearComposite() {
    delete root_dir_;
    directoryMap_.clear();
}

/**
 * @brief FileExplorerScene::Sync
 * Recreates the composite and loads the root scene when a sync is needed
 * @param directoriesArray Directories on the Bucket
 * @param filesArray Files in the bucket
 */
void FileExplorerScene::Sync(QJsonArray directoriesArray, QJsonArray filesArray) {
    ClearComposite();
    CreateDirectoryComposite(directoriesArray, filesArray);

    HandleSyncSignal(directoriesArray, filesArray);
    LoadScene(root_dir_);
    curr_dir_ = root_dir_;
}


/**
 * @brief FileExplorerScene::dragMoveEvent
 * Handles when a file to be uploaded is dragged around the scene
 * @param event Scene drag drop event
 */
void FileExplorerScene::dragMoveEvent(QGraphicsSceneDragDropEvent *event) {
    event->acceptProposedAction();
}

/**
 * @brief FileExplorerScene::dragEnterEvent
 * Handles when a file to be upload enters the scene
 * @param event Scene drag drop event
 */
void FileExplorerScene::dragEnterEvent(QGraphicsSceneDragDropEvent *event) {
    event->acceptProposedAction();
}

// https://wiki.qt.io/Drag_and_Drop_of_files
/**
 * @brief FileExplorerScene::dropEvent
 * Handles when a file is dropped into the scene
 * @param event Scene drag drop event
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

/**
 * @brief FileExplorerScene::AddFile
 * Adds the uploaded file to the Directory composite and to the sceen
 * @param filePath File path to the added file
 * @param md5 Md5 of the file
 */
void FileExplorerScene::AddFile(QString filePath, QString md5) {
    QStringList splitPath = filePath.split("/", QString::SkipEmptyParts);
    const QDir pathDir(filePath);
    QString relativePath;
    qDebug() << curr_dir_->getRelativePath();
    if(curr_dir_->getRelativePath() != "/") {
        relativePath = curr_dir_->getRelativePath() + splitPath[splitPath.length()-1];
    }
    else {
        relativePath = splitPath[splitPath.length()-1];
    }


    QString dropbucketDir = QDir::homePath() + "/Dropbucket" + "/" + relativePath;
    qDebug() << dropbucketDir;
    qDebug() << relativePath;
    if(!pathDir.exists() && !QFileInfo::exists(dropbucketDir)) {
        // Check it is a directory and that the file doesn't already exist
        // Create the file - add it to the directory
        Directory* file = factory_->createDir(0, 0, "file", splitPath[splitPath.length()-1], md5);
        connect(file, &Directory::Deleted, this, &FileExplorerScene::FileDeleted);
        connect(NetworkManager::getInstance(), &NetworkManager::FileDeleteSuccessfulSignal, file, &Directory::NetworkDeleteSuccessful);
        file->setRelativePath(relativePath);
        file->setParent(curr_dir_);
        curr_dir_->AddDir(file);
        curr_x_ += 50;
        LoadScene(curr_dir_);
        curr_x_ += 56;

        if(curr_x_ > 1200) {
            curr_x_ = 0;
            curr_y_ += 100;
        }

        NetworkManager::getInstance()->FilePost(filePath, curr_dir_->getRelativePath());
        FileAddedSignal(dropbucketDir);
    }
    else {
        // Will enter here when the file exists in Dropbucket dir
        QFile fileToSave(dropbucketDir);
        QFile *file = new QFile(filePath);

        file->open(QIODevice::ReadOnly);
        fileToSave.resize(file->size());
        QByteArray data = file->readAll();
        fileToSave.open(QIODevice::WriteOnly);
        fileToSave.write(data);
        file->close();
        fileToSave.close();
    }
}

/**
 * @brief FileExplorerScene::GetMimeType
 * Gets the Mime type from MimeData
 * @param inData Data to check
 * @return Mime type
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
        return "Couldn't determine MIME type";
    }
}

/**
 * @brief FileExplorerScene::getDirectoryKeys
 * Gets the directory keys from the directory map
 * @return Directory keys
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
 * Handles when a mouse is clicked on the scene.
 * @param event Mouse event
 */
void FileExplorerScene::mousePressEvent(QGraphicsSceneMouseEvent *event) {
    if(allowClicks_) {
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
}
