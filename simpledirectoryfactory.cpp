#include "simpledirectoryfactory.h"

SimpleDirectoryFactory::SimpleDirectoryFactory(){}

/**
 * @brief SimpleDirectoryFactory::createDir
 * @param x
 * @param y
 * @param type
 * @param name
 * @param md5
 * @return
 */
Directory* SimpleDirectoryFactory::createDir(int x, int y, QString type, QString name, QString md5) {
    if(type == "file") {
        // Create data file
        QPixmap pixmap("://icons/icon_file.png");
        DataFile* newDir = new DataFile(x, y, pixmap);

        newDir->setMd5(md5);
        newDir->setName(name);

        return newDir;
    }
    else if(type == "folder") {
        // Create folder

        QPixmap pixmap("://icons/icon_folder.png");
        Folder* newDir = new Folder(x, y, pixmap);

        newDir->setName(name);

        return newDir;
    }


    return NULL;
}
