// bdc 0x08834328 BtlHudGetNthPropTarget
#include "bdc.h"

/* Returns the `n`-th prop target of the battle unit list (`BtlGetBakuganList`): collects, in list
   order, the ids of the units other than the player unit (`BtlGetPlayerBakugan`) for which vtable
   slot 14 (`IsTargetPoint`, `BtlTargetPointIsTargetPoint`) and slot 15 (`IsPropTarget`,
   `BtlBakuganIsPropTarget`) both return non-zero, then looks id `n` up with
   `CoreObjectListFindById`. Returns NULL when there is no player unit, no list, or `n` is not
   below the count. The id buffer holds 21 entries with no bound check. `self` is unused. */

CoreObject *BtlHudGetNthPropTarget(BtlHud *self, s32 n)
{
    CoreObject **head;
    void *player;
    CoreObject *unit;
    const VtblEntry *vtbl;
    s32 count;
    u32 ids[21];

    (void)self;
    head = BtlGetBakuganList();
    player = BtlGetPlayerBakugan();
    if (player == NULL || head == NULL) {
        return NULL;
    }
    unit = *head;
    if (unit == NULL) {
        return NULL;
    }
    count = 0;
    for (; unit != NULL; unit = unit->next) {
        vtbl = (const VtblEntry *)unit->vtable;
        if (((int (*)(void *))vtbl[14].fn)((u8 *)unit + vtbl[14].delta) == 0 ||
            (void *)unit == player) {
            continue;
        }
        vtbl = (const VtblEntry *)unit->vtable;
        if (((int (*)(void *))vtbl[15].fn)((u8 *)unit + vtbl[15].delta) != 0) {
            ids[count++] = unit->id;
        }
    }
    if (n < count) {
        return CoreObjectListFindById(head, ids[n]);
    }
    return NULL;
}
