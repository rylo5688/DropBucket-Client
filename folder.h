#ifndef FOLDER_H
#define FOLDER_H

#include <QObject>
#include "directory.h"
#include "datafile.h"

class Folder : public Directory
{
public:
    enum { Type = UserType + 2 };

    int type() const override {
        return Type;
    }

    Folder(int x, int y, QPixmap pixmap, QGraphicsItem *parent = nullptr);

    void AddDir(Directory* dir) override;

    void RemoveDir(Directory* dir) override;

    std::vector<Directory*> getContents() const override;

private:
    std::vector<Directory*> contents_;
};

#endif // FOLDER_H
