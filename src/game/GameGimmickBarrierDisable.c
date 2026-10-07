// bdc 0x088d9dbc GameGimmickBarrierDisable
#include "bdc.h"

/* Vtable `0x08af33c4` slot 16: when the gimmick is active (`+0x15e`), clears it; with `immediate ==
   1` hides the attached object and zeroes the alpha `+0x6c` at once, otherwise plays sound
   `0x2c00015` (`SndManagerPlay`) and switches to state 1 (fade out). */

void GameGimmickBarrierDisable(GameGimmickBarrier *gimmick, u8 immediate)
{
  if (gimmick->base.active != 0) {
    gimmick->base.active = 0;
    if (immediate == 1) {
      GameGimmickBarrierDisableCollider(gimmick);
      gimmick->base.base.ambient[3] = 0.0f;
    } else {
      if (SndHasManager()) {
        SndManagerPlay(SndGetManager(), 0x2c00015, 0, 0);
      }
      gimmick->base.state = 1;
    }
  }
}
