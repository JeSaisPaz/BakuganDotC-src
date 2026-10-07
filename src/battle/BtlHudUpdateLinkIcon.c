// bdc 0x0882d658 BtlHudUpdateLinkIcon
#include "bdc.h"

/* Shows the linked-object icon (HUD sprite slot 0x76, `sprites` byte offset 0x1d8) while
   `BtlBakuganCanUseLinkedObject` is true and hides it (clears visible bit 0 of its flags)
   otherwise; on the frame it turns visible it adds 1 to statistic 0x15 of the Bakugan's `stats`
   record (`BtlStatsAddCounter`) when the unit has one. */

void BtlHudUpdateLinkIcon(BtlHud *self, BtlBakugan *unit)
{
  if (BtlBakuganCanUseLinkedObject(unit) == 0) {
    self->sprites[0x76]->flags &= ~1u;
  } else {
    if ((self->sprites[0x76]->flags & 1) == 0 && unit->stats != NULL) {
      BtlStatsAddCounter(unit->stats, 0x15, 1);
    }
    self->sprites[0x76]->flags |= 1;
  }
}
