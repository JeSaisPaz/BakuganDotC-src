// bdc 0x088d938c GameGimmickDraw
#include "bdc.h"

/* Draw method of the gimmick base (vtable `0x08af3314` slot 8): forwards to
   `GfxModelDlWriteState` unchanged. Derived gimmicks' draw methods call it. */

void GameGimmickDraw(GameGimmick *gimmick, u32 **dl)

{
  GfxModelDlWriteState(&gimmick->base,dl);
  return;
}

