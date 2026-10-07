// bdc 0x08866360 BtlFindBakuganById
#include "bdc.h"

/* Returns the unit of `g_btlBakuganList` with object id `id` (`CoreObjectListFindById`), or
   NULL for id 0. Attacks and the AI resolve their stored target ids with it. */

CoreObject *BtlFindBakuganById(u32 id)
{
  if (id != 0) {
    return CoreObjectListFindById(g_btlBakuganList, id);
  }
  return NULL;
}
