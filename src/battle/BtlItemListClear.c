// bdc 0x088b69e0 BtlItemListClear
#include "bdc.h"

/* Deletes every battle item (`CoreObjectListClear` on `g_btlItemList`). Called by
   `BtlMainTaskDtor` and `BtlMainTeardown`. */
void BtlItemListClear(void)
{
    CoreObjectListClear(&g_btlItemList);
}
