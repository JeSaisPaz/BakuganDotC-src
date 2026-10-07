// bdc 0x0883322c BtlHudGetNthOtherBakugan
#include "bdc.h"

/* Returns the `n`-th unit of `BtlGetBakuganList` other than the player Bakugan
   (`BtlGetPlayerBakugan`) that is a battle Bakugan (virtual slot 10 true, base vtable
   `0x08af1fa4`) or a `BtlUnitAlt` (virtual slot 12 true, vtable `0x08af1c94`),
   skipping TargetPoint units (virtual slot 14 true, `BtlTargetPointCtor`); the ids of the
   matches (up to 21 slots on the stack) are collected in chain order and the `n`-th is looked up
   again with `CoreObjectListFindById`. NULL when there are fewer, when there is no player unit
   or no list. `self` is unused. */
CoreObject *BtlHudGetNthOtherBakugan(BtlHud *self, s32 n)
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
        const VtblEntry *vtbl = obj->vtable;
        const VtblEntry *isTargetPoint = &vtbl[14];

        if (((s32 (*)(void *))isTargetPoint->fn)((u8 *)obj + isTargetPoint->delta) == 0 &&
            obj != player) {
            const VtblEntry *isBakugan = &vtbl[10];
            const VtblEntry *isUnitAlt = &vtbl[12];

            if (((s32 (*)(void *))isBakugan->fn)((u8 *)obj + isBakugan->delta) != 0 ||
                ((s32 (*)(void *))isUnitAlt->fn)((u8 *)obj + isUnitAlt->delta) != 0) {
                ids[count++] = obj->id;
            }
        }
        obj = obj->next;
    } while (obj != NULL);
    if (n < count) {
        return CoreObjectListFindById(head, ids[n]);
    }
    return NULL;
}
