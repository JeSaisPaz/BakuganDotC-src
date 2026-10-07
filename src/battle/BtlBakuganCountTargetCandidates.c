// bdc 0x08862f28 BtlBakuganCountTargetCandidates
#include "bdc.h"

/* Counts the units in `g_btlBakuganList` that `BtlBakuganPickTarget` would consider for `self`:
   skips `self`, dead units (`combat.dead`) and untargetable ones (status 9 active); with
   `mode == 1` also those whose virtual predicate in slot 17 (`+0x88`) returns non-zero; and when
   `self` is player-controlled (`isPlayer`) also those for which slot 15 (`+0x78`) or slot 16
   (`+0x80`) returns non-zero. Returns the count. */
s32 BtlBakuganCountTargetCandidates(BtlBakugan *self, s32 mode)
{
    BtlBakugan *unit;
    const VtblEntry *vtbl;
    s32 count = 0;

    for (unit = *(BtlBakugan **)g_btlBakuganList; unit != NULL;
         unit = (BtlBakugan *)unit->base.base.next) {
        if (unit == self || unit->combat.dead != 0 || unit->combat.status[9].active != 0) {
            continue;
        }
        if (mode == 1) {
            vtbl = (const VtblEntry *)unit->base.base.vtable;
            if (((int (*)(void *))vtbl[17].fn)((u8 *)unit + vtbl[17].delta) != 0) {
                continue;
            }
        }
        if (self->isPlayer != 0) {
            vtbl = (const VtblEntry *)unit->base.base.vtable;
            if (((int (*)(void *))vtbl[15].fn)((u8 *)unit + vtbl[15].delta) != 0) {
                continue;
            }
            vtbl = (const VtblEntry *)unit->base.base.vtable;
            if (((int (*)(void *))vtbl[16].fn)((u8 *)unit + vtbl[16].delta) != 0) {
                continue;
            }
        }
        count++;
    }
    return count;
}
