// bdc 0x088b6794 BtlItemListCount
#include "bdc.h"

/* Returns the number of items in `g_btlItemList`. */
int BtlItemListCount(void)
{
    int count = 0;
    CoreObject *item;

    for (item = g_btlItemList; item != NULL; item = item->next) {
        count++;
    }
    return count;
}
