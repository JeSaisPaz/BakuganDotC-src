// bdc 0x08866094 BtlSetBakuganList
#include "bdc.h"

/* Stores its argument in `g_btlBakuganList` (singleton accessor, named by `bdc singleton`). */

void BtlSetBakuganList(void *value)
{
  g_btlBakuganList = value;
}
