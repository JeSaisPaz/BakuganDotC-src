// bdc 0x088626dc BtlBakuganSetTarget
#include "bdc.h"

/* Sets the Bakugan's target: if `target` is in the battle chain
   (`CoreObjectListContains` on `g_btlBakuganList`) it stores the target's unique object id as
   `targetId` and returns 1, otherwise returns 0. */
int BtlBakuganSetTarget(BtlBakugan *self, void *target)
{
  if (CoreObjectListContains(g_btlBakuganList, target) != 0) {
    self->targetId = ((CoreObject *)target)->id;
    return 1;
  }
  return 0;
}
