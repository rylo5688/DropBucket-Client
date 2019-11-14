#include "simpledirectoryfactory.h"

SimpleDirectoryFactory::SimpleDirectoryFactory()
{

}


Directory* SimpleDirectoryFactory::createDir(int x, int y, std::string type, std::string name, std::string md5) {
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
