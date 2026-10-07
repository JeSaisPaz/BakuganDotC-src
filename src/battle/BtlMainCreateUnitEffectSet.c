// bdc 0x0884c130 BtlMainCreateUnitEffectSet
#include "bdc.h"

/* Replaces the battle main task's second (unit) effect manager `unitEffects`: deletes the old one
   through its `GfxSpriteLayer` vtable (entry 1, deleting destructor, flag 3) and clears the
   field, allocates 0xa0 bytes from the low heap under `MemLock`, constructs a `GfxEffectMgr`
   on `particleData` (`GfxEffectMgrCtor`) and stores the result (NULL when the allocation
   failed) in `unitEffects` and `g_btlUnitEffectMgr`. Unlike `BtlMainCreateStageEffectSet` it
   does not load the effect models first. */
void BtlMainCreateUnitEffectSet(BtlMain *self, s32 *particleData)
{
    GfxEffectMgr *old = (GfxEffectMgr *)self->unitEffects;
    GfxEffectMgr *mgr;
    bool fromLow;

    if (old != NULL) {
        const VtblEntry *dtor = &old->base.vtbl[1];

        ((void (*)(void *, s32))dtor->fn)((u8 *)old + dtor->delta, 3);
        self->unitEffects = NULL;
    }
    MemLock();
    fromLow = MemIsAllocFromLow();
    MemSetAllocFromLow(true);
    mgr = MemAlloc(sizeof(GfxEffectMgr), NULL, 0);
    MemSetAllocFromLow(fromLow);
    MemUnlock();
    if (mgr != NULL) {
        GfxEffectMgrCtor(mgr, particleData);
    }
    self->unitEffects = mgr;
    g_btlUnitEffectMgr = mgr;
}
