#ifndef SYNCOFF_H
#define SYNCOFF_H

#include <QObject>
#include "sync.h"
//#include <QMainWindow>
//#include "mainwindow.h"

class SyncOff : public Sync
{
    Q_OBJECT
public:
    SyncOff();

    void HandleSync() override {}

};

#endif // SYNCOFF_H
