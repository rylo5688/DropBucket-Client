#include "fileexplorerscene.h"

FileExplorerScene::FileExplorerScene()
{
//    QPixmap pixmap("://icons/icon_file.png");
//    addPixmap(pixmap);
    Q_INIT_RESOURCE(resources);
    LoadScene(QString::fromStdString("src"));
    factory_ = new SimpleDirectoryFactory();
}

void FileExplorerScene::AddIcon(int x, int y) {
    QPixmap pixmap("://icons/icon_file.png");
    Directory *test = new DataFile(x, y, pixmap);
    addItem(test);
    QGraphicsTextItem *temp = addText("test.txt");
    temp->setPos(QPointF(0,56));
    qreal textWidth = temp->textWidth();
    qDebug() << textWidth;
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
    // Read the JSON
    QJsonObject json = OpenReadJSON();
    CreateDirectoryComposite(json);


//    QJsonArray folders = currDir["dirs"].toArray();
//    QStringList folderNames = GetKeys(folders, "folder");
//    QJsonArray files = currDir["files"].toArray();
//    QStringList fileNames = GetKeys(files, "file");

//    qDebug() << folderNames;
//    qDebug() << fileNames;

    // Loop through the QStringLists and add icons


    // Get the path from input string - render out the necessary level
    AddIcon(0,0);
}

QJsonObject FileExplorerScene::OpenReadJSON() {
    Q_INIT_RESOURCE(resources);
    QFile loadFile(QStringLiteral("://jsons/test_dir.json"));

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

void FileExplorerScene::CreateDirectoryComposite(QJsonObject &json) {
    std::vector<std::pair<QJsonObject, std::vector<QJsonObject>>> directories; // Directory to subdirectories

    std::vector<QJsonObject> visited;
    std::queue<QJsonObject> queue;
    queue.push(json);
    QJsonArray::iterator it;
    Directory *parent;
    bool root = true;
    while(!queue.empty()) {
        QJsonObject currDir = queue.front();
        QStringList keys = currDir.keys();
        qDebug() << keys[0] << currDir;
        queue.pop();

        // Create root directory first
//        if(root) {
//            Directory* rootDir = factory_->createDir(0, 0, "folder", keys[0].toStdString());
//            root_dir_ = rootDir;
//            parent = rootDir;
//            root = false;
//        }
//        else {
//            // Find the parent in the
//        }

        QJsonObject trimCurrDir = currDir[keys[0]].toObject();
        qDebug() << trimCurrDir;

        QJsonArray subDirs;
        subDirs = trimCurrDir["dirs"].toArray();
//        qDebug() << subDirs;
        std::vector<QJsonObject> sub;
        for(it = subDirs.begin(); it != subDirs.end(); it++) {
            QJsonObject newDir = (*it).toObject();
            QStringList childKeys = newDir.keys();
            qDebug() << "child: " << newDir;
            sub.push_back(newDir);
            if(!checkInVisited(&visited, newDir)) {
                visited.push_back(newDir);
                queue.push(newDir);
            }
        }

        directories.push_back(std::pair<QJsonObject, std::vector<QJsonObject>>(currDir, sub));
    }

   for(int i = 0; i < directories.size(); i++) {
       qDebug() << i << ": " << directories[i].first;
       for(int j = 0; j < directories[i].second.size(); j++) {
           qDebug() << "       " << j << ": " << directories[i].second[j];
       }
   }
}

void FileExplorerScene::mousePressEvent(QGraphicsSceneMouseEvent *event) {
    QGraphicsScene::mousePressEvent(event);
    if(itemAt(event->scenePos(), QTransform()) != nullptr) {
        QGraphicsItem *clicked = itemAt(event->scenePos(), QTransform());
        qDebug() << clicked;
    }
}
