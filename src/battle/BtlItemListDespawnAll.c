// bdc 0x088b6804 BtlItemListDespawnAll
#include "bdc.h"

/* Despawns every pickup item chained from `g_btlItemList` with `BtlItemDespawn`; the next link
   is read before each call. Called by `BtlDemoStateLoad`. */
void BtlItemListDespawnAll(void)
{
    CoreObject *item;
    CoreObject *next;

    item = g_btlItemList;
    while (item != NULL) {
        next = item->next;
        BtlItemDespawn(item);
        item = next;
    }
}
