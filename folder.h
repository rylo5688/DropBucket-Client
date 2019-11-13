#ifndef FOLDER_H
#define FOLDER_H

#include <QObject>
#include "directory.h"
#include "datafile.h"

class Folder : public Directory
{
public:
    Folder(int x, int y, QPixmap pixmap, QGraphicsItem *parent = nullptr);

    void AddDir(Directory* dir);

    void RemoveDir(Directory* dir);

private:
    std::vector<Directory*> contents_;
};

#endif // FOLDER_H
