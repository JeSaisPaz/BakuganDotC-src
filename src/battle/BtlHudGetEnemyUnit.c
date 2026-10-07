// bdc 0x08830498 BtlHudGetEnemyUnit
#include "bdc.h"

typedef s32 (*BtlUnitClassTest)(void *self);

/* Calls class predicate `slot` of a battle-list object through its vtable. */
static s32 BtlHudUnitIs(CoreObject *obj, s32 slot)
{
    const VtblEntry *test = &((const VtblEntry *)obj->vtable)[slot];
    return ((BtlUnitClassTest)test->fn)((u8 *)obj + test->delta);
}

/* Returns the `n`-th opponent unit of the battle list: objects other than the
   player's Bakugan that are not target points (vtable slot 14), are Bakugan
   or alt units (slot 10 or slot 12) and whose combat state is not dead,
   collected by id (at most 21) and resolved with CoreObjectListFindById.
   NULL when there is no player, no list, or fewer than n + 1 such units. */
void *BtlHudGetEnemyUnit(BtlHud *self, s32 n)
{
    CoreObject **head;
    CoreObject *player;
    CoreObject *obj;
    s32 count;
    u32 ids[21];

    (void)self;
    head = BtlGetBakuganList();
    player = BtlGetPlayerBakugan();
    if (player == NULL || head == NULL) {
        return NULL;
    }
    obj = *head;
    if (obj == NULL) {
        return NULL;
    }
    count = 0;
    do {
        if (!BtlHudUnitIs(obj, 14) && obj != player
            && (BtlHudUnitIs(obj, 10) || BtlHudUnitIs(obj, 12))
            && !((BtlBakugan *)obj)->combat.dead) {
            ids[count++] = obj->id;
        }
        obj = obj->next;
    } while (obj != NULL);
    if (n < count) {
        return CoreObjectListFindById(head, ids[n]);
    }
    return NULL;
}
