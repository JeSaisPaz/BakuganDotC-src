// bdc 0x08866150 BtlPickRandomOtherFighter
#include "bdc.h"

/* Collects every unit of `g_btlBakuganList` whose `playerSlot` is below 5 and that is not
   `exclude` into a 32-entry local array, and returns one of them at random (`CoreRandNext`), or
   NULL when there is none (or the list is missing). The array is not bounds-checked. Used by
   `BtlStageUpdateAmbientEffects`. */
void *BtlPickRandomOtherFighter(void *exclude)
{
    CoreObject *found[32];
    CoreObject *obj = NULL;
    s32 count = 0;

    if (g_btlBakuganList != NULL) {
        obj = ((CoreObjectList *)g_btlBakuganList)->head;
    }
    for (; obj != NULL; obj = obj->next) {
        if (((BtlBakugan *)obj)->playerSlot < 5 && (void *)obj != exclude) {
            found[count] = obj;
            count++;
        }
    }
    if (count > 0) {
        return found[CoreRandNext((u32)count)];
    }
    return NULL;
}
