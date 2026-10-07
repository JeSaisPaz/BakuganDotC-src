// bdc 0x088ffcd4 BtlHasDuplicateSpecies
#include "bdc.h"

/* Returns true when the battle rule mode (`g_scriptGlobalVars` entry 8) is 2 and some unit in the
   battle unit list (`BtlGetBakuganList`) has an earlier unit of the same species
   (`BtlBakuganCountEarlierSameSpecies` > 0); walks the whole list. False when the list is
   missing or the mode is not 2. */
bool BtlHasDuplicateSpecies(void)
{
    CoreObjectList *list = (CoreObjectList *)BtlGetBakuganList();
    CoreObject *obj;
    bool found;

    if (list == NULL || g_scriptGlobalVars[8] != 2) {
        return false;
    }
    found = false;
    for (obj = list->head; obj != NULL; obj = obj->next) {
        if (BtlBakuganCountEarlierSameSpecies((BtlBakugan *)obj) > 0) {
            found = true;
        }
    }
    return found;
}
