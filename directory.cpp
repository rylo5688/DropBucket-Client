#include "directory.h"

/**
 * @brief Directory::Directory
 * Constructor for Directory
 * @param x X location
 * @param y Y location
 * @param pixmap Icon for files
 * @param parent Parent
 */
Directory::Directory(int x, int y, QPixmap pixmap, QGraphicsItem *parent) : QGraphicsPixmapItem(pixmap.scaled(QSize(56,56)), parent)
{
    this->x = x;
    this->y = y;
    parent = nullptr;
}

/**
 * @brief Directory::boundingRect
 * Sets the bouding rectangle for the icon
 * @return QRectF bounding rectange
 */
QRectF Directory::boundingRect() const {
    return QRectF(x, y, 56, 56);
}

/**
 * @brief Directory::shape
 * Sets the shape of the bounding box
 * @return QPainterPath for the bounding box
 */
QPainterPath Directory::shape() const {
    QPainterPath path;
    path.addRect(boundingRect());

    return path;
}

/**
 * @brief Directory::paint
 * Paints the icon and text below it
 * @param painter QPainter used for painting
 * @param option QStyleOptionGraphicsItem options for the style
 * @param widget unused paramater
 */
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

/**
 * @brief Directory::getParent
 * Gets the directory's parent
 * @return parent_ of the directory
 */
Directory* Directory::getParent() {
    return parent_ ;
}

/**
 * @brief Directory::getName
 * Gets the directory name
 * @return name of the directory
 */
QString Directory::getName() const {
    return name;
}

/**
 * @brief Directory::getRelativePath
 * Gets the directory's relative path
 * @return relative path of the directory
 */
QString Directory::getRelativePath() const {
    return relativePath_;
}

/**
 * @brief Directory::setName
 * Sets the directory's name
 * @param n Name to set
 */
void Directory::setName(QString n) {
    name = n;
}

/**
 * @brief Directory::setRelativePath
 * Sets the directory's relative path
 * @param path Relative path
 */
void Directory::setRelativePath(QString path) {
    relativePath_ = path;
}

/**
 * @brief Directory::setParent
 * Sets the Directory's parent
 * @param parent Parent
 *
 */
void Directory::setParent(Directory *parent) {
    parent_ = parent;
}
