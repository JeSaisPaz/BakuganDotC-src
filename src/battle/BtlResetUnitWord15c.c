// bdc 0x0884cce8 BtlResetUnitWord15c
#include "bdc.h"

/* Clears the target id (`targetId`, word `+0x15c`) of every unit in `g_btlBakuganList` whose
   vtable entry 14 (`BtlBakuganIsTargetPoint` in the base class) returns 0, i.e. of the real
   Bakugan units. Called from jump-table cases of `BtlMainPhaseFinish`. */
void BtlResetUnitWord15c(void)
{
    CoreObjectList *list = (CoreObjectList *)BtlGetBakuganList();
    CoreObject *obj;

    if (list == NULL) {
        return;
    }
    for (obj = list->head; obj != NULL; obj = obj->next) {
        const VtblEntry *isTargetPoint = &((const VtblEntry *)obj->vtable)[14];

        if (((int (*)(void *))isTargetPoint->fn)((u8 *)obj + isTargetPoint->delta) == 0) {
            ((BtlBakugan *)obj)->targetId = 0;
        }
    }
}
