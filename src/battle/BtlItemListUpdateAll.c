// bdc 0x088b75bc BtlItemListUpdateAll
#include "bdc.h"

/* Runs `BtlItemUpdate` on every item of `g_btlItemList` (next pointer read first, so items may
   delete themselves). Called by `BtlMainUpdateScene` and `BtlMainPhaseSceneOnly`. */
void BtlItemListUpdateAll(void)
{
    CoreObject *item = g_btlItemList;
    CoreObject *next;

    while (item != NULL) {
        next = item->next;
        BtlItemUpdate(item);
        item = next;
    }
}
