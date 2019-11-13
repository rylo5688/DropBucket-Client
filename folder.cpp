#include "folder.h"

Folder::Folder(int x, int y, QPixmap pixmap, QGraphicsItem *parent) : Directory(x, y, pixmap, parent)
{ }

void Folder::AddDir(Directory* dir) {
    contents_.push_back(dir);
}

void Folder::RemoveDir(Directory* dir) {
    std::vector<Directory*>::iterator it;
    for(it = contents_.begin(); it != contents_.end(); it++) {
        if(*it == dir) {
            contents_.erase(it);

            return;
        }
    }
}
