#ifndef DATAFILE_H
#define DATAFILE_H

#include <QObject>
#include <QMenu>
#include <QAction>
#include <QGraphicsSceneContextMenuEvent>
#include "directory.h"
#include "networkmanager.h"

class DataFile : public Directory
{
    Q_OBJECT
public:
    enum { Type = UserType + 3 };

    int type() const override {
        return Type;
    }

    DataFile(int x, int y, QPixmap pixmap, QGraphicsItem *parent = nullptr);

    QString getMd5() const;

    void setMd5(QString md5);

    void contextMenuEvent(QGraphicsSceneContextMenuEvent * event) override;

private slots:
    void DeleteFile();

private:
    QString md5; // Will be hex

};

#endif // DATAFILE_H
