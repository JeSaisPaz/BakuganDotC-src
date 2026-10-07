// bdc 0x0884c054 BtlMainCreateStageEffectSet
#include "bdc.h"

/* Replaces the battle main task's stage effect manager `stageEffects`: reloads the shared effect
   models (`GfxEffectModelsLoad`), deletes the old manager through its vtable entry 1 (deleting
   destructor, flag 3), allocates 0xa0 bytes from the low end of the heap (`MemAlloc` under
   `MemLock`, previous placement policy restored), constructs a `GfxEffectMgr` on the particle
   data `data` (`GfxEffectMgrCtor`) and stores the result (NULL when the allocation failed) in
   `stageEffects` and `g_worldEffectMgr`. */
void BtlMainCreateStageEffectSet(BtlMain *self, s32 *data)
{
    GfxEffectMgr *old;
    GfxEffectMgr *mgr;
    bool fromLow;

    GfxEffectModelsLoad();
    old = (GfxEffectMgr *)self->stageEffects;
    if (old != NULL) {
        const VtblEntry *dtor = &old->base.vtbl[1];
        ((void (*)(void *, s32))dtor->fn)((u8 *)old + dtor->delta, 3);
        self->stageEffects = NULL;
    }
    MemLock();
    fromLow = MemIsAllocFromLow();
    MemSetAllocFromLow(true);
    mgr = MemAlloc(sizeof(GfxEffectMgr), NULL, 0);
    MemSetAllocFromLow(fromLow);
    MemUnlock();
    if (mgr != NULL) {
        GfxEffectMgrCtor(mgr, data);
    }
    self->stageEffects = mgr;
    g_worldEffectMgr = mgr;
}
