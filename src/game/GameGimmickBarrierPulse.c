// bdc 0x088d9c8c GameGimmickBarrierPulse
#include "bdc.h"

/* Every 4th frame (counter `+0x190`) moves the node-callback value `+0x180` by 0.25 between 0 and
   0.75, reversing at the ends (direction flag `+0x194`). */

void GameGimmickBarrierPulse(GameGimmickBarrier *gimmick)
{
  float v;

  gimmick->pulseTimer++;
  if ((gimmick->pulseTimer & 3) == 0) {
    if (gimmick->pulseDown == 0) {
      v = gimmick->texOffset[0] + 0.25f;
      gimmick->texOffset[0] = v;
      if (!(v < 0.75f)) {
        gimmick->pulseDown = 1;
        return;
      }
    } else {
      v = gimmick->texOffset[0] - 0.25f;
      gimmick->texOffset[0] = v;
      if (!(v <= 0.0f)) {
        return;
      }
      gimmick->pulseDown = 0;
    }
  }
}
