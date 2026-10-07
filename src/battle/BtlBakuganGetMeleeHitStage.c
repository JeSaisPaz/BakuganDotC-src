// bdc 0x088644cc BtlBakuganGetMeleeHitStage
#include "bdc.h"

/* Returns the combo stage of a basic melee hit id: for ids below 0x12, `id % 3` gives stage 1/2/3
   (0 → 1, 1 → 2, 2 → 3); any other id returns 1. `BtlBakuganOnHit` rotates the knock-back
   direction for stages 2 and 3. */

int BtlBakuganGetMeleeHitStage(BtlBakugan *self, int hitId)
{
  int stage = 1;

  (void)self;
  if (hitId < 0x12) {
    if (hitId % 3 == 1) {
      return 2;
    }
    if (hitId % 3 == 2) {
      stage = 3;
    }
  }
  return stage;
}
