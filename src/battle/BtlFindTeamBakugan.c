// bdc 0x0882cfe0 BtlFindTeamBakugan
#include "bdc.h"

/* Returns the battle unit of player slot `slot` (-1 = the active slot, profile word 0x13 via
   `SaveProfileGetWord`, resolved only when the list exists): walks `BtlGetBakuganList` and, for
   the first unit that is a Bakugan (vtable slot 10, `BtlBakuganIsBakugan`) or a BtlUnitAlt (slot
   12, `BtlBakuganIsUnitAlt`) whose `playerSlot` equals `slot`, returns
   `BtlBakuganListFind``(unit)`. NULL when the list is missing or no unit matches. Used by the
   battle demos and HUD result screens. */

void *BtlFindTeamBakugan(u32 slot)
{
    BtlBakugan **list;
    BtlBakugan *unit;
    const VtblEntry *vtbl;

    list = BtlGetBakuganList();
    if (list == NULL) {
        return NULL;
    }
    unit = *list;
    if (slot == 0xffffffffu) {
        slot = SaveProfileGetWord(SaveGetProfile(), 0x13);
    }
    for (; unit != NULL; unit = (BtlBakugan *)unit->base.base.next) {
        vtbl = (const VtblEntry *)unit->base.base.vtable;
        if (((int (*)(void *))vtbl[10].fn)((u8 *)unit + vtbl[10].delta) == 0) {
            vtbl = (const VtblEntry *)unit->base.base.vtable;
            if (((int (*)(void *))vtbl[12].fn)((u8 *)unit + vtbl[12].delta) == 0) {
                continue;
            }
        }
        if ((u32)unit->playerSlot == slot) {
            return BtlBakuganListFind(unit);
        }
    }
    return NULL;
}
