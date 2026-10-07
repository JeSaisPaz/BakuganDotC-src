// bdc 0x08866088 BtlGetBakuganList
#include "bdc.h"

/* Returns `g_btlBakuganList`, the head object of the battle's Bakugan object chain; NULL
   outside a battle. */
void *BtlGetBakuganList(void)
{
    return g_btlBakuganList;
}
