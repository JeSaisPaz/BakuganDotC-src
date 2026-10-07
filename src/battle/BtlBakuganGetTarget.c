// bdc 0x08860304 BtlBakuganGetTarget
#include "bdc.h"

/* Returns the Bakugan's current target object: when targetId is non-zero, looks it up in
   `g_btlBakuganList` with `CoreObjectListFindById`, else NULL. */

void *BtlBakuganGetTarget(BtlBakugan *bakugan)
{
  if (bakugan->targetId != 0) {
    return CoreObjectListFindById(g_btlBakuganList, bakugan->targetId);
  }
  return NULL;
}
