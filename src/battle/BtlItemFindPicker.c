// bdc 0x088b68c8 BtlItemFindPicker
#include "bdc.h"

/* Finds who picks up a battle item: while the battle is undecided (`g_btlBattleOutcome` == 0),
   walks the Bakugan list (`BtlGetBakuganList`) and takes the first unit that is not dead, whose
   vtable entry 10 (can pick up) returns non-zero and whose squared distance to the item (over
   x/y/z) is <= 57600 (240 units). Stores it in `item->picker` and returns 1; else
   returns 0 and leaves `picker` unchanged. */
int BtlItemFindPicker(BtlItem *item)
{
    CoreObjectList *list = (CoreObjectList *)BtlGetBakuganList();
    CoreObject *obj;

    if (g_btlBattleOutcome != 0 || list == NULL) {
        return 0;
    }
    for (obj = list->head; obj != NULL; obj = obj->next) {
        BtlBakugan *unit = (BtlBakugan *)obj;
        const VtblEntry *canPickUp;
        float dx, dy, dz, distSq;

        if (unit->combat.dead) {
            continue;
        }
        canPickUp = &((const VtblEntry *)obj->vtable)[10];
        if (((int (*)(void *))canPickUp->fn)((u8 *)obj + canPickUp->delta) == 0) {
            continue;
        }
        dx = unit->base.pos[0] - item->pos[0];
        dy = unit->base.pos[1] - item->pos[1];
        dz = unit->base.pos[2] - item->pos[2];
        distSq = dx * dx + dy * dy + dz * dz;
        if (distSq <= 57600.0f) {
            item->picker = unit;
            return 1;
        }
    }
    return 0;
}
