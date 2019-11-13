#ifndef DATAFILE_H
#define DATAFILE_H

#include <QObject>
#include "directory.h"

class DataFile : public Directory
{
public:
    DataFile(int x, int y, QPixmap pixmap, QGraphicsItem *parent = nullptr);

    std::string getMd5() const;

    void setMd5(std::string md5);

    void onClick();

private:
    std::string md5;
};

#endif // DATAFILE_H
