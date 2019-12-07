#include "directory.h"

Directory::Directory(int x, int y, QPixmap pixmap, QGraphicsItem *parent) : QGraphicsPixmapItem(pixmap.scaled(QSize(56,56)), parent)
{
    this->x = x;
    this->y = y;
    parent = nullptr;
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
    if(name.size() > 10) {
        painter->drawText(QPoint(x + 10, y + 65), name.mid(0,10) + "...");
    }
    else {
        painter->drawText(QPoint(x + 10, y + 65), name);
    }
}

Directory* Directory::getParent() {
    return parent_ ;
}

QString Directory::getName() const {
    return name;
}

QString Directory::getRelativePath() const {
    return relativePath_;
}

void Directory::setName(QString n) {
    name = n;
}

void Directory::setRelativePath(QString path) {
    relativePath_ = path;
}

void Directory::setParent(Directory *parent) {
    parent_ = parent;
}
