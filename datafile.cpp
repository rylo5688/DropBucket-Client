#include "datafile.h"

/**
 * @brief DataFile::DataFile
 * Constructor for DataFile
 * @param x X location
 * @param y Y location
 * @param pixmap Icon for files
 * @param parent Parent
 */
DataFile::DataFile(int x, int y, QPixmap pixmap, QGraphicsItem *parent) : Directory(x, y, pixmap, parent)
{
}

/**
 * @brief DataFile::getMd5
 * Retrieves file md5
 * @return md5 QString
 */
QString DataFile::getMd5() const {
    return md5;
}

/**
 * @brief DataFile::setMd5
 * Sets MD5
 * @param md5 Md5 to set
 */
void DataFile::setMd5(QString md5) {
    this->md5 = md5;
}

/**
 * @brief DataFile::contextMenuEvent
 * Handles when the context menu when a file is right clicked
 * @param event Context menu event
 */
void DataFile::contextMenuEvent(QGraphicsSceneContextMenuEvent *event) {
    QMenu menu;
    QAction *deleteAct = menu.addAction("Delete");
    connect(deleteAct, &QAction::triggered, this, &DataFile::DeleteFile);
    menu.exec(event->screenPos());
}

/**
 * @brief DataFile::DeleteFile
 * Slot for when the file is deleted from context menu
 */
void DataFile::DeleteFile() {
    NetworkManager::getInstance()->FileDelete(relativePath_);
    parent_->RemoveDir(this);

    Deleted(this);
    qDebug() << "sent signal";

    this->deleteLater();
}
