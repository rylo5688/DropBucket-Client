#include "directory.h"

Directory::Directory(int x, int y, QPixmap pixmap, QGraphicsItem *parent) : QGraphicsPixmapItem(pixmap.scaled(QSize(56,56)), parent)
{
    this->x = x;
    this->y = y;
    setPos(QPointF(x,y));
}

QRectF Directory::boundingRect() const {
    return QRectF(x, y, 56, 56);
}

QPainterPath Directory::shape() const {
    QPainterPath path;
    path.addRect(boundingRect());

    return path;
}

void Directory::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) {
    Q_UNUSED(widget);

    painter->drawPixmap(QPointF(x,y), pixmap(), boundingRect());
}

std::string Directory::getName() const {
    return name;
}

void Directory::setName(std::string n) {
    name = n;
}
