// bdc 0x088662bc BtlCountAliveBakugan
#include "bdc.h"

/* Counts the units in `g_btlBakuganList` that are not dead (combat `dead` byte == 0, as in
   `BtlBakuganAreOthersAllDead`) and for which one of two virtual predicates holds: vtable
   entry 10 (`+0x50`), or, when that returns 0, entry 12 (`+0x60`). */
s32 BtlCountAliveBakugan(void)
{
    BtlBakugan *unit;
    s32 count = 0;

    for (unit = *(BtlBakugan **)g_btlBakuganList; unit != NULL;
         unit = (BtlBakugan *)unit->base.base.next) {
        const VtblEntry *first = &((const VtblEntry *)unit->base.base.vtable)[10];

        if (((int (*)(void *))first->fn)((u8 *)unit + first->delta) == 0) {
            const VtblEntry *second = &((const VtblEntry *)unit->base.base.vtable)[12];

            if (((int (*)(void *))second->fn)((u8 *)unit + second->delta) == 0) {
                continue;
            }
        }
        if (unit->combat.dead == 0) {
            count++;
        }
    }
    return count;
}
