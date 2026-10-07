// bdc 0x088b6648 BtlItemDtor
#include "bdc.h"

/* Destructor (slot 1 of `g_btlItemVtbl`) of a battle pickup `BtlItem` (`BtlItemCtor`): for a
   non-NULL item, reinstalls `g_btlItemVtbl`; when `effect` (`+0x48`) is set, stops every effect
   attached to its `pos` (`+0x20`) on `g_btlItemEffectMgr` (`GfxEffectStopAttached`, any id);
   deletes its `model` (`+0x4c`) through the model's virtual deleting destructor (vtable entry 1,
   flags 3); runs `CoreObjectDtor``(self, 0)` and frees the item (`MemFree` under `MemLock`)
   when bit 0 of `flags` is set. */

void BtlItemDtor(BtlItem *self, u32 flags)
{
    GfxModel *model;

    if (self == NULL) {
        return;
    }
    self->base.vtable = g_btlItemVtbl;
    if (self->effect != NULL) {
        GfxEffectStopAttached(g_btlItemEffectMgr, -1, self->pos);
    }
    model = self->model;
    if (model != NULL) {
        const VtblEntry *dtor = &((const VtblEntry *)model->base.vtable)[1];

        ((void (*)(void *, s32))dtor->fn)((u8 *)model + dtor->delta, 3);
    }
    CoreObjectDtor(&self->base, 0);
    if (flags & 1) {
        MemLock();
        MemFree(self, NULL, 0);
        MemUnlock();
    }
}
