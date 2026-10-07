// bdc 0x08845c98 BtlArenaPhotoTaskCtor
#include "bdc.h"

/* Constructor of the arena opponent-portrait loader task `BtlArenaPhotoTask` (task id 108):
   `CoreTaskInit`, installs `g_btlArenaPhotoTaskVtbl`, clears `g_btlArenaPhotoDone`, the step
   and the three portrait slots (load, path, sprite). Returns `task`. */
BtlArenaPhotoTask *BtlArenaPhotoTaskCtor(BtlArenaPhotoTask *task)
{
    int i;

    CoreTaskInit(&task->base);
    task->base.vtable = g_btlArenaPhotoTaskVtbl;
    g_btlArenaPhotoDone = 0;
    task->step = 0;
    for (i = 0; i < 3; i++) {
        task->loads[i] = NULL;
        task->paths[i] = NULL;
        task->sprites[i] = NULL;
    }
    return task;
}
