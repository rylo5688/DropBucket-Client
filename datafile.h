#ifndef DATAFILE_H
#define DATAFILE_H

#include <QObject>
#include <QMenu>
#include <QAction>
#include <QGraphicsSceneContextMenuEvent>
#include "directory.h"

class DataFile : public QObject, public Directory
{
    Q_OBJECT
public:
    enum { Type = UserType + 3 };

    int type() const override {
        return Type;
    }

    DataFile(int x, int y, QPixmap pixmap, QGraphicsItem *parent = nullptr);

    std::string getMd5() const;

    void setMd5(std::string md5);

    void contextMenuEvent(QGraphicsSceneContextMenuEvent * event) override;

private slots:
    void DeleteFile();

private:
    std::string md5;

};

#endif // DATAFILE_H
