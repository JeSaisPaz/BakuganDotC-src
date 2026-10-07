// bdc 0x08834438 BtlHudGetNthLandmarkAttrTarget
#include "bdc.h"

/* Returns the `n`-th unit of the battle unit list (`BtlGetBakuganList`), in list order, other
   than the player unit (`BtlGetPlayerBakugan`) for which vtable entry 14 (the TargetPoint test,
   `BtlTargetPointIsTargetPoint`) and vtable entry 16 (the landmark-attr test,
   `BtlTargetPointLandmarkAttrIsLandmarkAttrTarget`) both return non-zero, i.e. the companion
   targets of hologram/attribute landmarks (`BtlTargetPointLandmarkAttrCtor`). It collects the
   matching unit ids into a 21-entry stack buffer (no bound check) and resolves entry `n` with
   `CoreObjectListFindById`; NULL when there are not more than `n` matches, or when the player
   unit or the list is missing. Used by `BtlHudUpdateRadar` for 3 blips (`sprites+0x33c`). */

CoreObject *BtlHudGetNthLandmarkAttrTarget(BtlHud *self, s32 n)
{
    CoreObjectList *list;
    CoreObject *player;
    CoreObject *obj;
    s32 count;
    u32 ids[21];

    (void)self;
    list = (CoreObjectList *)BtlGetBakuganList();
    player = (CoreObject *)BtlGetPlayerBakugan();
    if (player == NULL || list == NULL) {
        return NULL;
    }
    count = 0;
    obj = list->head;
    if (obj == NULL) {
        return NULL;
    }
    do {
        const VtblEntry *isTargetPoint = &((const VtblEntry *)obj->vtable)[14];

        if (((int (*)(void *))isTargetPoint->fn)((u8 *)obj + isTargetPoint->delta) != 0 &&
            obj != player) {
            const VtblEntry *isLandmarkAttr = &((const VtblEntry *)obj->vtable)[16];

            if (((int (*)(void *))isLandmarkAttr->fn)((u8 *)obj + isLandmarkAttr->delta) != 0) {
                ids[count] = obj->id;
                count++;
            }
        }
        obj = obj->next;
    } while (obj != NULL);
    if (n < count) {
        return CoreObjectListFindById(&list->head, ids[n]);
    }
    return NULL;
}
