#include "directory.h"

Directory::Directory(int x, int y, QPixmap pixmap, QGraphicsItem *parent) : QGraphicsPixmapItem(pixmap.scaled(QSize(56,56)), parent)
{
    this->x = x;
    this->y = y;
}

QRectF Directory::boundingRect() const {
    return QRectF(x, y, 56, 56);
}

std::string Directory::getName() const {
    return name;
}

void Directory::setName(std::string n) {
    name = n;
}
