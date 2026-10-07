// bdc 0x08846550 BtlTutorialSetActionMasks
#include "bdc.h"

/* For tutorial step `step` 0 or 0xd, sets input->allowedActions of every unit in the Bakugan chain
   whose virtual predicate (vtable entry 18) returns non-zero: local players get -1 (everything),
   other units the step's mask from g_btlTutorialActionMasks, with bits 0x4004 cleared for kinds
   0x15..0x20. Other steps, or no chain, change nothing. `task` is unused. */
void BtlTutorialSetActionMasks(void *task, int step)
{
    s32 steps[2];
    CoreObjectList *list;
    BtlBakugan *unit;
    s32 i;
    u32 mask;

    (void)task;
    steps[0] = 0;
    steps[1] = 0xd;
    list = BtlGetBakuganList();
    if (list == NULL) {
        return;
    }
    for (i = 0; i < 2; i++) {
        if (steps[i] != step) {
            continue;
        }
        for (unit = (BtlBakugan *)list->head; unit != NULL; unit = (BtlBakugan *)unit->base.base.next) {
            const VtblEntry *pred = &((const VtblEntry *)unit->base.base.vtable)[18];

            if (((s32 (*)(void *))pred->fn)((u8 *)unit + pred->delta) == 0) {
                continue;
            }
            if (BtlBakuganIsLocalPlayer(unit)) {
                unit->input->allowedActions = 0xffffffff;
            } else {
                mask = g_btlTutorialActionMasks[i];
                if (unit->base.base.unk08 > 0x14 && unit->base.base.unk08 <= 0x20) {
                    mask &= ~0x4004u;
                }
                unit->input->allowedActions = mask;
            }
        }
    }
}
