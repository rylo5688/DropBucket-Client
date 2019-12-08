#ifndef SYNCOFF_H
#define SYNCOFF_H

#include <QObject>
#include "sync.h"


// This is a Null Object as it doesn't contain any code and does nothing when instantiated

class SyncOff : public Sync
{
    Q_OBJECT
public:
    SyncOff();
};

#endif // SYNCOFF_H
