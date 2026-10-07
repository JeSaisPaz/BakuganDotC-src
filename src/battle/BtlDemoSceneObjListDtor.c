// bdc 0x08904c90 BtlDemoSceneObjListDtor
#include "bdc.h"

/* Destructor of a scene object list head: only frees the head when bit 0 of `flags` is set (the
   entries are deleted by `BtlDemoSceneDeleteObjects`). */
void BtlDemoSceneObjListDtor(void *list, u32 flags)
{
    if (list != NULL && (flags & 1) != 0) {
        MemLock();
        MemFree(list, NULL, 0);
        MemUnlock();
    }
}
