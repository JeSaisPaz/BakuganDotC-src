// bdc 0x0893bce8 UiUnlockResultCardTypePopDone
#include "bdc.h"

/* Advances the pops of sprites 0x1c/0x1d on `UiUnlockResult` for reward kind
   `+0x5ee` 1; true at once otherwise. */

bool UiUnlockResultCardTypePopDone(UiUnlockResult *self, u8 closing)
{
  u8 a;
  u8 b;

  switch (self->rewardKind) {
  case 1:
    a = UiUnlockResultPopSprite(self, closing, 0x1d);
    b = UiUnlockResultPopSprite(self, closing, 0x1c);
    return (u8)(a + b) != 0;
  default:
    return true;
  }
}
