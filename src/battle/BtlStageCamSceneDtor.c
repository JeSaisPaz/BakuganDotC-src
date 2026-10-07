// bdc 0x08904924 BtlStageCamSceneDtor
#include "bdc.h"

/* Destructor of a stage camera scene (`BtlStageCamScene`): does nothing for NULL; otherwise frees
   its buffers (`BtlStageCamSceneUnload`) and, when bit 0 of `flags` is set, frees the object
   itself (`MemFree` under `MemLock`). */
void BtlStageCamSceneDtor(BtlStageCamScene *scene, u32 flags)
{
    if (scene == NULL) {
        return;
    }
    BtlStageCamSceneUnload(scene);
    if ((flags & 1) != 0) {
        MemLock();
        MemFree(scene, NULL, 0);
        MemUnlock();
    }
}
