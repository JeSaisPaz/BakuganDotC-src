// bdc 0x088c6cac GameFieldCameraProbeDtor
#include "bdc.h"

/* Destructor of the camera collision probe (`GameFieldCameraProbeCtor`): frees it only when
   `flags & 1`. */
void GameFieldCameraProbeDtor(void *probe, u32 flags)
{
    if (probe != NULL && (flags & 1) != 0) {
        MemLock();
        MemFree(probe, NULL, 0);
        MemUnlock();
    }
}
