// bdc 0x088b6764 BtlItemListCountKind
#include "bdc.h"

/* Returns how many items of type `kind` are in `g_btlItemList`. */
int BtlItemListCountKind(s32 kind)
{
    int count = 0;
    CoreObject *node;

    for (node = g_btlItemList; node != NULL; node = node->next) {
        if (((BtlItem *)node)->type == kind) {
            count++;
        }
    }
    return count;
}
