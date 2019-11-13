#include "datafile.h"

DataFile::DataFile(int x, int y, QPixmap pixmap, QGraphicsItem *parent) : Directory(x, y, pixmap, parent)
{

}

void DataFile::onClick() {

}

std::string DataFile::getMd5() const {
    return md5;
}

void DataFile::setMd5(std::string md5) {
    this->md5 = md5;
}

