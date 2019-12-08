#ifndef SIMPLEDIRECTORYFACTORY_H
#define SIMPLEDIRECTORYFACTORY_H

#include <QObject>
#include <QDebug>
#include "directory.h"
#include "datafile.h"
#include "folder.h"

// This is a Simple Factory pattern

class SimpleDirectoryFactory
{
public:
    SimpleDirectoryFactory();

    Directory* createDir(int x, int y, QString type, QString name, QString md5 = "");


};

#endif // SIMPLEDIRECTORYFACTORY_H
