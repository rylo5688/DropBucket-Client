#ifndef DATAFILE_H
#define DATAFILE_H

#include <QObject>
#include "directory.h"

class DataFile : public Directory
{
public:
    enum { Type = UserType + 3 };

    int type() const override {
        return Type;
    }

    DataFile(int x, int y, QPixmap pixmap, QGraphicsItem *parent = nullptr);

    std::string getMd5() const;

    void setMd5(std::string md5);

private:
    std::string md5;

};

#endif // DATAFILE_H
