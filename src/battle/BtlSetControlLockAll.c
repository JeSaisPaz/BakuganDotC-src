// bdc 0x0884b404 BtlSetControlLockAll
#include "bdc.h"

/* Stores `lock` in `g_btlControlLockAll`; when the unit list (`BtlGetBakuganList`) exists,
   calls `BtlBakuganSetControlLock``(unit, lock)` for every unit whose vtable entry 14 returns 0
   and that has an input controller, then stores `lock` in `g_btlControlLockApplied` (left
   unchanged without a list). Used with 1 to freeze the units during scripted camera shots and
   demos, and with 0 to release them. */
void BtlSetControlLockAll(u8 lock)
{
    CoreObjectList *list = (CoreObjectList *)BtlGetBakuganList();
    CoreObject *obj;

    g_btlControlLockAll = lock;
    if (list == NULL) {
        return;
    }
    for (obj = list->head; obj != NULL; obj = obj->next) {
        const VtblEntry *isTargetPoint = &((const VtblEntry *)obj->vtable)[14];

        if (((int (*)(void *))isTargetPoint->fn)((u8 *)obj + isTargetPoint->delta) == 0 &&
            ((BtlBakugan *)obj)->input != NULL) {
            BtlBakuganSetControlLock((BtlBakugan *)obj, lock);
        }
    }
    g_btlControlLockApplied = lock;
}
