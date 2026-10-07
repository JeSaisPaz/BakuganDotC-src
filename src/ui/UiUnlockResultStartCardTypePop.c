// bdc 0x0893bbe8 UiUnlockResultStartCardTypePop
#include "bdc.h"

/* For reward kind `+0x5ee` 1 (card) on `UiUnlockResult`: shows sprites
   0x1c/0x1d, sets sprite 0x1c's cell row to `+0x7ec` / 4 (`GfxSpriteSetCell`) and begins both
   pops; closing begins the reverse pops. */

void UiUnlockResultStartCardTypePop(UiUnlockResult *self, u8 closing)

{
  GfxSprite **sprites;

  switch(self->rewardKind) {
  case 1:
    if (closing == 0) {
      sprites = (GfxSprite **)self->base.data;
      sprites[29]->flags |= 1;
      sprites[28]->flags |= 1;
      GfxSpriteSetCell(sprites[28], 0.0f, (float)(self->rewardIndex / 4));
      UiUnlockResultBeginPop(self, 0, 0x1d);
      UiUnlockResultBeginPop(self, 0, 0x1c);
    } else {
      UiUnlockResultBeginPop(self, closing, 0x1d);
      UiUnlockResultBeginPop(self, closing, 0x1c);
    }
    break;
  default:
    break;
  }
}
