// bdc 0x088daa28 GameGimmickEffectMarkerDraw
#include "bdc.h"

/* Draw method of the effect-marker gimmick (vtable `0x08af3524` slot 8): forwards to
   `GfxModelDlWriteState`. */

void GameGimmickEffectMarkerDraw(GameGimmickEffectMarker *gimmick, u32 **dl)

{
  GfxModelDlWriteState((GfxModel *)gimmick,dl);
  return;
}

