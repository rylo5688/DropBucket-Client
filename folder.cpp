#include "folder.h"

/**
 * @brief Folder::Folder
 * Folder constructor
 * @param x X location
 * @param y Y location
 * @param pixmap Icon pixmap
 * @param parent Parent
 */
Folder::Folder(int x, int y, QPixmap pixmap, QGraphicsItem *parent) : Directory(x, y, pixmap, parent)
{
    setCursor(Qt::PointingHandCursor);
}

/**
 * @brief Folder::AddDir
 * Adds a directory to the folder
 * @param dir Directory to add
 */
void Folder::AddDir(Directory* dir) {
    contents_.push_back(dir);
}

/**
 * @brief Folder::RemoveDir
 * Removes a directory from a folder
 * @param dir Directory to remove
 */
void Folder::RemoveDir(Directory* dir) {
    std::vector<Directory*>::iterator it;
    for(it = contents_.begin(); it != contents_.end(); it++) {
        if(*it == dir) {
            contents_.erase(it);

            return;
        }
    }
}
