// bdc 0x0893a818 UiUnlockResultStartSprite05Pop
#include "bdc.h"

/* For card-type rewards (reward kind `+0x5ee` 1, 2, 3, 7) on `UiUnlockResult`:
   shows sprite 5 and begins its pop when opening, or the reverse pop when closing. */

void UiUnlockResultStartSprite05Pop(UiUnlockResult *self, u8 closing)
{
  GfxSprite **sprites;

  switch (self->rewardKind) {
  case 1:
  case 2:
  case 3:
  case 7:
    if (closing != 0) {
      UiUnlockResultBeginPop(self, closing, 5);
      return;
    }
    sprites = (GfxSprite **)self->base.data;
    sprites[5]->flags = sprites[5]->flags | 1;
    UiUnlockResultBeginPop(self, 0, 5);
    return;
  default:
    return;
  }
}
