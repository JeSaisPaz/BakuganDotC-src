// bdc 0x08854680 BtlCutInTaskCtor
#include "bdc.h"

/* Constructor of the battle cut-in task (`BtlCutInTask`, id 481 = 0x1e1, created through
   `CoreTaskNewByIdArg` by `BtlMainStartCutIn`): base `CoreTaskInit`, installs
   `g_btlCutInTaskVtbl`, stores `unit` and clears `frame`; allocates a 0xa0-byte effect manager
   from the low heap end (under `MemLock`, restoring the previous placement policy) and, when
   that succeeded, builds it on `"particle_00.ptb"` (`CorePackChainFind`, `GfxEffectMgrCtor`)
   into `effects` (NULL otherwise). Spawns cut-in effect 0x85 (0x86 when
   `BtlCameraIsTargetOnLeft` holds for `g_gfxActiveCamera`) attached to the camera's `target`
   point, sets the effect's `textureSlot` from the unit's virtual slot 20, updates the manager once
   (`GfxEffectMgrUpdate`) and clears `g_btlCameraDefaultMode`. Returns the task. */

CoreTask *BtlCutInTaskCtor(BtlCutInTask *self, BtlBakugan *unit)
{
    bool fromLow;
    GfxEffectMgr *mgr;
    GfxEffectMgr *effects;
    GfxEffect *effect;
    const VtblEntry *vtbl;
    int id;

    CoreTaskInit(&self->base);
    self->base.vtable = g_btlCutInTaskVtbl;
    self->unit = unit;
    self->frame = 0;
    MemLock();
    fromLow = MemIsAllocFromLow();
    MemSetAllocFromLow(true);
    mgr = MemAlloc(sizeof(GfxEffectMgr), NULL, 0);
    MemSetAllocFromLow(fromLow);
    MemUnlock();
    effects = NULL;
    if (mgr != NULL) {
        GfxEffectMgrCtor(mgr, CorePackChainFind(g_ioLzsPackages, "particle_00.ptb"));
        effects = mgr;
    }
    self->effects = effects;
    id = 0x85;
    if (BtlCameraIsTargetOnLeft((BtlCamera *)g_gfxActiveCamera)) {
        id = 0x86;
    }
    effect = GfxEffectSpawnAttached(self->effects, id, g_gfxActiveCamera->target);
    vtbl = (const VtblEntry *)unit->base.base.vtable;
    effect->textureSlot = ((s32 (*)(void *))vtbl[20].fn)((u8 *)unit + vtbl[20].delta);
    GfxEffectMgrUpdate(self->effects);
    g_btlCameraDefaultMode = 0;
    return &self->base;
}
