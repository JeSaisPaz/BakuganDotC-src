// bdc 0x0893a30c UiUnlockResultStartHologramSprite07Pop
#include "bdc.h"

/* For reward kind `+0x5ee` 2 (hologram) on `UiUnlockResult`: shows sprite 7
   and begins its pop (open), or begins the reverse pop (close). */

void UiUnlockResultStartHologramSprite07Pop(UiUnlockResult *self, u8 closing)
{
  GfxSprite **sprites;

  if (closing == 0) {
    if (self->rewardKind == 2) {
      sprites = (GfxSprite **)self->base.data;
      sprites[7]->flags = sprites[7]->flags | 1;
      UiUnlockResultBeginPop(self, 0, 7);
    }
  } else if (self->rewardKind == 2) {
    UiUnlockResultBeginPop(self, closing, 7);
  }
}
