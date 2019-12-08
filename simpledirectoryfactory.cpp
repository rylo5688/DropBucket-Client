#include "simpledirectoryfactory.h"

SimpleDirectoryFactory::SimpleDirectoryFactory(){}

/**
 * @brief SimpleDirectoryFactory::createDir
 * Creates a directory of a specified type
 * @param x X location
 * @param y Y location
 * @param type Type of directory (file or folder)
 * @param name Name of the directory
 * @param md5 Md5 if a file
 * @return Created directory
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


    return nullptr;
}
