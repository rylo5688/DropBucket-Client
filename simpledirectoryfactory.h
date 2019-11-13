#ifndef SIMPLEDIRECTORYFACTORY_H
#define SIMPLEDIRECTORYFACTORY_H

#include <QObject>
#include "directory.h"
#include "datafile.h"
#include "folder.h"

class SimpleDirectoryFactory
{
public:
    SimpleDirectoryFactory();

    Directory* createDir(int x, int y, std::string type, std::string name, std::string md5 = nullptr);


};

#endif // SIMPLEDIRECTORYFACTORY_H
