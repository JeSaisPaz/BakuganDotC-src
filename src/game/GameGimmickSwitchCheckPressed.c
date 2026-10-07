// bdc 0x088db964 GameGimmickSwitchCheckPressed
#include "bdc.h"

/* While `active` and the pressed bit (`contactFlags & 1`) is still clear, ticks
   `pressCollider` with `CollisionColliderTickHit` and on a hit sets bit 0
   (mask 0x1) of `contactFlags`. */

void GameGimmickSwitchCheckPressed(GameGimmickSwitch *gimmick)

{
  if (gimmick->base.active != 0 && (gimmick->base.contactFlags & 1) == 0 &&
      CollisionColliderTickHit(gimmick->pressCollider)) {
    gimmick->base.contactFlags |= 1;
  }
  return;
}

