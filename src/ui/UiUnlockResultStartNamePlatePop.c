// bdc 0x0893a928 UiUnlockResultStartNamePlatePop
#include "bdc.h"

/* For rewards with a name (reward kind `+0x5ee` 1, 2, 6, 8, 9) on
   `UiUnlockResult`: shows the name plate sprite 1 and begins its pop (open) or
   the reverse pop (close). */

void UiUnlockResultStartNamePlatePop(UiUnlockResult *self, u8 closing)
{
  GfxSprite **sprites;

  switch (self->rewardKind) {
  case 1:
  case 2:
  case 6:
  case 8:
  case 9:
    if (closing == 0) {
      sprites = (GfxSprite **)self->base.data;
      sprites[1]->flags = sprites[1]->flags | 1;
      UiUnlockResultBeginPop(self, 0, 1);
      return;
    }
    UiUnlockResultBeginPop(self, closing, 1);
    return;
  default:
    return;
  }
}
