#ifndef SYNC_H
#define SYNC_H

#include <QObject>

class Sync
{
public:
    Sync();

    virtual void HandleSync() {}
};

#endif // SYNC_H
