#ifndef FOLDER_H
#define FOLDER_H

#include <QObject>
#include <QApplication>
#include <QGraphicsSceneHoverEvent>
#include "directory.h"
#include "datafile.h"

class Folder : public Directory
{
    Q_OBJECT
public:
    enum { Type = UserType + 2 };

    int type() const override {
        return Type;
    }

    Folder(int x, int y, QPixmap pixmap, QGraphicsItem *parent = nullptr);

    void AddDir(Directory* dir) override;

    void RemoveDir(Directory* dir) override;

private:
};

#endif // FOLDER_H
