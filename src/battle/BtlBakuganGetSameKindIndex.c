// bdc 0x08845e3c BtlBakuganGetSameKindIndex
#include "bdc.h"

/* In game mode 2 (script global 8, `g_scriptGlobalVars`), returns how many units before `self`
   in the battle unit list (`BtlGetBakuganList`) have the same kind (CoreObject word `+8`), i.e.
   the duplicate index used for the `arena_com_pho_%03d_%02d` photo variant. Returns 0 when the
   list is missing, `self` is NULL or the mode is not 2; if `self` is not in the list, counts every
   matching unit. */
int BtlBakuganGetSameKindIndex(BtlBakugan *self)
{
    CoreObjectList *list = (CoreObjectList *)BtlGetBakuganList();
    CoreObject *obj;
    int count;

    if (list == NULL || self == NULL || g_scriptGlobalVars[8] != 2) {
        return 0;
    }
    count = 0;
    for (obj = list->head; obj != NULL && obj != &self->base.base; obj = obj->next) {
        if (obj->unk08 == self->base.base.unk08) {
            count++;
        }
    }
    return count;
}
