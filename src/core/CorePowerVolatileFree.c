// bdc 0x089bcbfc CorePowerVolatileFree
#include "bdc.h"

/* Frees `ptr` back to the volatile-memory heap under the "COPower" lock by calling the heap's free
   method (`MemMng2` vtable entry 2: moves the block from the used to the address-sorted free
   list and merges it with free neighbours). Returns 1 when the heap accepted the block, else 0
   (also when the heap does not exist). */
int CorePowerVolatileFree(CorePower *power, void *ptr)
{
    int freed = 0;
    MemMng2 *heap;

    (void)power;
    CoreLockAcquire(g_corePowerMgr->lock);
    heap = g_corePowerMgr->volatileHeap;
    if (heap != NULL) {
        const VtblEntry *free_ = &heap->vtbl[2];
        if (((int (*)(void *, void *))free_->fn)((u8 *)heap + free_->delta, ptr) != 0) {
            freed = 1;
        }
    }
    CoreLockRelease(g_corePowerMgr->lock);
    return freed;
}
