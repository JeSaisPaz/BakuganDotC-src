// bdc 0x08862d30 BtlBakuganStopHitReaction
#include "bdc.h"

/* Ends a battle unit's hit reaction at once: zeroes the hit-shake intensity `hitShake`
   (`BtlBakuganUpdateHitShake` stops shaking the model) and the hit `cooldown` of the unit's
   body collider `collider0` (counted down by `CollisionColliderTickHit`). Returns nothing. */

void BtlBakuganStopHitReaction(BtlBakugan *bakugan)
{
    bakugan->hitShake = 0.0f;
    bakugan->collider0->cooldown = 0;
}
