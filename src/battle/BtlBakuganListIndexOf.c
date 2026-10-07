// bdc 0x0886358c BtlBakuganListIndexOf
#include "bdc.h"

/* Returns the 0-based position of `self` in the battle Bakugan chain `g_btlBakuganList` (walked
   through the `next` links); returns the list length when it is not found. */
int BtlBakuganListIndexOf(BtlBakugan *self)
{
    int index = 0;
    BtlBakugan *node;

    for (node = *(BtlBakugan **)g_btlBakuganList; node != NULL && node != self;
         node = (BtlBakugan *)node->base.base.next) {
        index++;
    }
    return index;
}
