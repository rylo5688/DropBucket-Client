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
    painter->drawText(QPoint(x + 10, y + 65), QString::fromStdString(name));
}

Directory* Directory::getParent() {
    return parent;
}

std::string Directory::getName() const {
    return name;
}

std::string Directory::getRelativePath() const {
    return relativePath_;
}

void Directory::setName(std::string n) {
    name = n;
}

void Directory::setRelativePath(std::string path) {
    relativePath_ = path;
}

void Directory::setParent(Directory *parent) {
    this->parent = parent;
}
