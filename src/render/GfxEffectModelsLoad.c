// bdc 0x088251c4 GfxEffectModelsLoad
#include "bdc.h"

/* (Re)loads the shared effect models: for each of the `g_gfxEffectModelCount` names in
   `g_gfxEffectModelNames` deletes the previous model in `g_gfxEffectModels``[i]` (vtable
   entry 1, deleting destructor, flag 3) and stores a new 0x140-byte `GfxModel`
   allocated from the low end of the heap (NULL when the allocation failed). Called by
   `BtlMainCreateStageEffectSet` and `GameFieldCreateEffectManager`. */
void GfxEffectModelsLoad(void)
{
    GfxModel **slot;
    const char *const *name;
    GfxModel *old;
    GfxModel *model;
    bool fromLow;
    s32 i;

    slot = g_gfxEffectModels;
    name = g_gfxEffectModelNames;
    for (i = 0; i < g_gfxEffectModelCount; i++, slot++, name++) {
        old = *slot;
        if (old != NULL) {
            const VtblEntry *dtor = &((const VtblEntry *)old->base.vtable)[1];
            ((void (*)(void *, s32))dtor->fn)((u8 *)old + dtor->delta, 3);
            *slot = NULL;
        }
        MemLock();
        fromLow = MemIsAllocFromLow();
        MemSetAllocFromLow(true);
        model = MemAlloc(sizeof(GfxModel), NULL, 0);
        MemSetAllocFromLow(fromLow);
        MemUnlock();
        if (model != NULL) {
            GfxModelCtor(model, *name, 0);
        }
        *slot = model;
    }
}
