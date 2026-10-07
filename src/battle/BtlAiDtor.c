// bdc 0x0888cc6c BtlAiDtor
#include "bdc.h"

/* Destructor of the CPU AI object (`BtlAi`, 0xa30 bytes): does nothing for NULL;
   otherwise destroys the per-species parameter objects (`BtlAiReleaseKindParams`), drops one user
   of the rule-script cache (`BtlAiComScriptCacheRelease`), empties the two delay
   `BtlAiWeightTable`s `delayWeights[1]` then `[0]` (a borrowed array is only detached, an owned
   one is freed with `MemFree`), destroys the combo table (`BtlAiComboTableDtor`), the four
   command channels (`CxxVecDelete` with `BtlAiChannelDtor`) and the two virtual-pad inputs
   (`BtlInputDtor` on `pad.prev`, then `pad.cur`), restores the base vtable `g_btlAiLayerVtbl` on
   the five behaviour layers (follow and seek-item pass through their own vtables first) and frees
   the object when bit 0 of `flags` is set. */

static void BtlAiDtorEmptyTable(BtlAiWeightTable *table)
{
    s32 *weights;

    if (table->borrowed != 0) {
        table->weights = NULL;
        table->borrowed = 0;
    }
    weights = table->weights;
    if (weights != NULL) {
        MemLock();
        MemFree(weights, NULL, 0);
        MemUnlock();
        table->weights = NULL;
    }
    table->count = 0;
    table->total = 0;
    table->totalValid = 0;
}

void BtlAiDtor(BtlAi *self, u32 flags)
{
    if (self == NULL) {
        return;
    }
    BtlAiReleaseKindParams(self->scriptCache, (void **)&self->params);
    BtlAiComScriptCacheRelease();
    BtlAiDtorEmptyTable(&self->delayWeights[1]);
    BtlAiDtorEmptyTable(&self->delayWeights[0]);
    BtlAiComboTableDtor(&self->comboTable, 2);
    CxxVecDelete(self->channels, 4, sizeof(BtlAiChannel), BtlAiChannelDtor, 0, 0);
    BtlInputDtor(&self->pad.prev, 2);
    BtlInputDtor(&self->pad.cur, 2);
    self->think.vtbl = g_btlAiLayerVtbl;
    self->wander.base.vtbl = g_btlAiLayerVtbl;
    self->seekItem.base.vtbl = g_btlAiSeekItemLayerVtbl;
    self->seekItem.base.vtbl = g_btlAiLayerVtbl;
    self->follow.base.vtbl = g_btlAiFollowLayerVtbl;
    self->follow.base.vtbl = g_btlAiLayerVtbl;
    self->guard.base.vtbl = g_btlAiLayerVtbl;
    if (flags & 1) {
        MemLock();
        MemFree(self, NULL, 0);
        MemUnlock();
    }
}
