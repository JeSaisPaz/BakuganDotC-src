// bdc 0x089bcb40 CorePowerVolatileAlloc
#include "bdc.h"

/* Allocates from the volatile-memory heap (`CorePowerMgr``.volatileHeap`) under the "COPower"
   lock: while the power state is 0 (running) and the heap exists it calls the heap's alloc method
   (`MemMng2` vtable entry 1) with `(size, file, line)` and returns the block (NULL on failure or
   when not running). The block lives in the 4 MiB region the PSP takes away on suspend, so users
   must free it from a suspend callback (`CorePowerAddSuspendCallback`). */
void *CorePowerVolatileAlloc(CorePower *power, u32 size, const char *file, int line)
{
    void *block = NULL;
    MemMng2 *heap;

    CoreLockAcquire(g_corePowerMgr->lock);
    if (power->state == 0) {
        heap = g_corePowerMgr->volatileHeap;
        if (heap != NULL) {
            const VtblEntry *alloc = &heap->vtbl[1];
            block = ((void *(*)(void *, u32, const char *, int))alloc->fn)(
                (u8 *)heap + alloc->delta, size, file, line);
        }
    }
    CoreLockRelease(g_corePowerMgr->lock);
    return block;
}
