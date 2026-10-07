// bdc 0x089bcc84 CorePowerIsVolatilePtr
#include "bdc.h"

/* Returns 1 when `ptr` lies inside the volatile-memory heap, asking the heap's range-check method
   (`MemMng2` vtable entry 3: `base <= ptr <= base + size`) under the "COPower" lock; 0 when the
   heap does not exist or the address is outside. Used to decide whether a buffer needs the
   volatile-memory handling of `CorePowerSafeMemcpy`. */
int CorePowerIsVolatilePtr(CorePower *power, void *ptr)
{
    int inside = 0;
    MemMng2 *heap;

    (void)power;
    CoreLockAcquire(g_corePowerMgr->lock);
    heap = g_corePowerMgr->volatileHeap;
    if (heap != NULL) {
        const VtblEntry *contains = &heap->vtbl[3];
        if (((int (*)(void *, void *))contains->fn)((u8 *)heap + contains->delta, ptr) != 0) {
            inside = 1;
        }
    }
    CoreLockRelease(g_corePowerMgr->lock);
    return inside;
}
