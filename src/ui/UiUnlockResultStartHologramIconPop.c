// bdc 0x0893a200 UiUnlockResultStartHologramIconPop
#include "bdc.h"

/* For reward kind `+0x5ee` 2 (hologram) on `UiUnlockResult`: when opening sets
   sprite 6's cell from `+0x636` (5-column sheet, `GfxSpriteSetCell`), shows it and begins its pop
   (`UiUnlockResultBeginPop`); when closing begins the reverse pop. */

void UiUnlockResultStartHologramIconPop(UiUnlockResult *self, u8 closing)
{
  GfxSprite **sprites;

  if (closing == 0) {
    if (self->rewardKind == 2) {
      GfxSpriteSetCell(((GfxSprite **)self->base.data)[6], (float)(self->holoIconCell / 5),
                       (float)(self->holoIconCell % 5));
      sprites = (GfxSprite **)self->base.data;
      sprites[6]->flags = sprites[6]->flags | 1;
      UiUnlockResultBeginPop(self, 0, 6);
    }
  } else if (self->rewardKind == 2) {
    UiUnlockResultBeginPop(self, closing, 6);
  }
}
