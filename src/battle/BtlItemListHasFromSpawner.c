// bdc 0x088b67b8 BtlItemListHasFromSpawner
#include "bdc.h"

/* Returns 1 if `g_btlItemList` holds an item whose spawner is `spawner` and that is not yet
   picked up, else 0. */
int BtlItemListHasFromSpawner(void *spawner)
{
    CoreObject *node;

    for (node = g_btlItemList; node != NULL; node = node->next) {
        BtlItem *item = (BtlItem *)node;

        if (item->pickedUp == 0 && item->spawner == spawner) {
            return 1;
        }
    }
    return 0;
}
