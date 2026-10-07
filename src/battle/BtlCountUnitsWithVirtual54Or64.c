// bdc 0x08866230 BtlCountUnitsWithVirtual54Or64
#include "bdc.h"

/* Counts the units of `g_btlBakuganList` for which unit virtual `+0x54` (vtable entry 10) or,
   only when that returned 0, virtual `+0x64` (entry 12) returns non-zero. In the base Bakugan
   vtable `0x08af1fa4` entry 10 is `BtlBakuganIsBakugan` (returns 1) and entry 12 is
   `BtlBakuganIsUnitAlt` (returns 0), so every unit keeping the base entry 10 is counted. */
int BtlCountUnitsWithVirtual54Or64(void)
{
    BtlBakugan *unit;
    int count = 0;

    for (unit = *(BtlBakugan **)g_btlBakuganList; unit != NULL;
         unit = (BtlBakugan *)unit->base.base.next) {
        const VtblEntry *first = &((const VtblEntry *)unit->base.base.vtable)[10];
        const VtblEntry *second;

        if (((int (*)(void *))first->fn)((u8 *)unit + first->delta) != 0) {
            count++;
            continue;
        }
        second = &((const VtblEntry *)unit->base.base.vtable)[12];
        if (((int (*)(void *))second->fn)((u8 *)unit + second->delta) != 0) {
            count++;
        }
    }
    return count;
}
