// bdc 0x088db1f4 GameGimmickTouchSpotCheckContact
#include "bdc.h"

/* While active and not yet touched, polls the collider `+0x174` for contact (`CollisionColliderTickHit`); on
   the first contact plays sound `0x2c0000e` and sets bit 1 of `+0x15f`. */

void GameGimmickTouchSpotCheckContact(GameGimmickTouchSpot *gimmick)
{
  if (gimmick->base.active != 0 && (gimmick->base.contactFlags & 1) == 0 &&
      CollisionColliderTickHit(gimmick->base.attached)) {
    if (SndHasManager()) {
      SndManagerPlay(SndGetManager(), 0x2c0000e, 0, 0);
    }
    gimmick->base.contactFlags |= 1;
  }
}
