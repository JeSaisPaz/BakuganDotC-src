// bdc 0x0884652c BtlArenaPhotoTaskUpdate
#include "bdc.h"

/* Update (`g_btlArenaPhotoTaskVtbl` entry 2) of the arena portrait loader: runs the load step
   machine `BtlArenaPhotoLoadStep` and stores its 'finished' result (low byte) in
   `g_btlArenaPhotoDone`. */
void BtlArenaPhotoTaskUpdate(BtlArenaPhotoTask *task)
{
    g_btlArenaPhotoDone = (u8)BtlArenaPhotoLoadStep(task);
}
