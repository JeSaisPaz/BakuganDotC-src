// bdc 0x088db940 GameGimmickSwitchUpdateGlow
#include "bdc.h"

/* Sets the glow node value `+0x180` to -0.25 while the switch is active (`+0x15e`) and 0 otherwise.
    */

void GameGimmickSwitchUpdateGlow(GameGimmickSwitch *gimmick)

{
  if ((gimmick->base).active != '\0') {
    gimmick->glow[0] = -0.25f;
    return;
  }
  gimmick->glow[0] = 0.0f;
  return;
}

