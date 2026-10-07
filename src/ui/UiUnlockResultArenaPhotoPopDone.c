// bdc 0x0893bec8 UiUnlockResultArenaPhotoPopDone
#include "bdc.h"

/* Advances the pops of sprites 0x1f/0x20 on `UiUnlockResult` for reward kind
   `+0x5ee` 9; true at once otherwise. */

bool UiUnlockResultArenaPhotoPopDone(UiUnlockResult *self, u8 closing)
{
  u8 a;
  u8 b;

  switch (self->rewardKind) {
  case 9:
    a = UiUnlockResultPopSprite(self, closing, 0x1f);
    b = UiUnlockResultPopSprite(self, closing, 0x20);
    return (u8)(a + b) != 0;
  default:
    return true;
  }
}
