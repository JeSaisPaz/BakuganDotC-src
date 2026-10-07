// bdc 0x0884b494 BtlStopAllUnitLoopSounds
#include "bdc.h"

/* For every unit in the battle unit list (`BtlGetBakuganList`) whose vtable entry 14
   returns 0 and that has an input controller, stops its sounds with
   `BtlBakuganStopLoopSounds``(unit, all)`. `main` is unused. */

void BtlStopAllUnitLoopSounds(void *main, bool all)
{
    CoreObjectList *list = (CoreObjectList *)BtlGetBakuganList();
    CoreObject *obj;

    (void)main;
    if (list == NULL) {
        return;
    }
    for (obj = list->head; obj != NULL; obj = obj->next) {
        const VtblEntry *isTargetPoint = &((const VtblEntry *)obj->vtable)[14];

        if (((int (*)(void *))isTargetPoint->fn)((u8 *)obj + isTargetPoint->delta) == 0 &&
            ((BtlBakugan *)obj)->input != NULL) {
            BtlBakuganStopLoopSounds((BtlBakugan *)obj, all);
        }
    }
}
