// bdc 0x0889c750 BtlAiCreateKindParamSet
#include "bdc.h"

/* Builds the three per-species parameter objects of a CPU AI (`out` = `ai+0x2cc`, three pointers):
   calls `BtlAiKindToScriptIndex` (result unused), does nothing more when `out` is NULL,
   otherwise releases the old objects (`BtlAiReleaseKindParams`), stores
   `BtlAiCreateKindParams``(mgr, kind)` in `out[0]`, then allocates two 8-byte `BtlAiParamSet`
   objects from the low end of the heap: `out[1]`/`out[2]` get `g_btlAiParamSetSlot1Vtbl` /
   `g_btlAiParamSetSlot2Vtbl` for kinds outside 0x15..0x20 and `g_btlAiParamSetSlot1NpcVtbl` /
   `g_btlAiParamSetSlot2NpcVtbl` for kinds 0x15..0x20 (the NPC unit kinds). A failed allocation
   leaves NULL in its slot. */

static inline BtlAiParamSet *BtlAiNewParamSet(s32 kind, const VtblEntry *derived)
{
    BtlAiParamSet *set;
    bool wasLow;

    MemLock();
    wasLow = MemIsAllocFromLow();
    MemSetAllocFromLow(true);
    set = MemAlloc(sizeof(BtlAiParamSet), NULL, 0);
    MemSetAllocFromLow(wasLow);
    MemUnlock();
    if (set != NULL) {
        set->vtbl = g_btlAiParamSetVtbl;
        set->kind = kind;
        set->vtbl = derived;
    }
    return set;
}

void BtlAiCreateKindParamSet(void *mgr, void **out, s32 kind)
{
    BtlAiKindToScriptIndex(kind);
    if (out == NULL) {
        return;
    }
    BtlAiReleaseKindParams(mgr, out);
    out[0] = BtlAiCreateKindParams(mgr, kind);
    if (kind < 0x15 || kind > 0x20) {
        out[1] = BtlAiNewParamSet(kind, g_btlAiParamSetSlot1Vtbl);
        out[2] = BtlAiNewParamSet(kind, g_btlAiParamSetSlot2Vtbl);
    } else {
        out[1] = BtlAiNewParamSet(kind, g_btlAiParamSetSlot1NpcVtbl);
        out[2] = BtlAiNewParamSet(kind, g_btlAiParamSetSlot2NpcVtbl);
    }
}
