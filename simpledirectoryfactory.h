#ifndef SIMPLEDIRECTORYFACTORY_H
#define SIMPLEDIRECTORYFACTORY_H

#include <QObject>
#include <QDebug>
#include "directory.h"
#include "datafile.h"
#include "folder.h"

class SimpleDirectoryFactory
{
public:
    SimpleDirectoryFactory();

    Directory* createDir(int x, int y, std::string type, std::string name, std::string md5 = "");


};

#endif // SIMPLEDIRECTORYFACTORY_H
