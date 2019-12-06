#include "datafile.h"

DataFile::DataFile(int x, int y, QPixmap pixmap, QGraphicsItem *parent) : Directory(x, y, pixmap, parent)
{

}

QString DataFile::getMd5() const {
    return md5;
}

void DataFile::setMd5(QString md5) {
    this->md5 = md5;
}

void DataFile::contextMenuEvent(QGraphicsSceneContextMenuEvent *event) {
    QMenu menu;
    QAction *deleteAct = menu.addAction("Delete");
    connect(deleteAct, &QAction::triggered, this, &DataFile::DeleteFile);
    menu.exec(event->screenPos());
}

void DataFile::DeleteFile() {
    qDebug() << "delte!";
    NetworkManager::getInstance()->FileDelete(relativePath_);
}
