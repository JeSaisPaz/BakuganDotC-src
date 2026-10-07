// bdc 0x0893bddc UiUnlockResultStartArenaPhotoPop
#include "bdc.h"

/* For reward kind `+0x5ee` 9 on `UiUnlockResult`: shows the photo sprite 0x1f
   with arena photo `+0x7ec` + 1 (`UiUnlockResultSetArenaPhoto`) and its frame sprite 0x20, and
   begins both pops; closing begins the reverse pops. Other kinds do nothing. */

void UiUnlockResultStartArenaPhotoPop(UiUnlockResult *self, u8 closing)
{
  GfxSprite **sprites;

  if (self->rewardKind != 9) {
    return;
  }
  if (closing == 0) {
    sprites = (GfxSprite **)self->base.data;
    sprites[0x1f]->flags = sprites[0x1f]->flags | 1;
    UiUnlockResultSetArenaPhoto(self, ((GfxSprite **)self->base.data)[0x1f],
                                (u8)(self->rewardIndex + 1));
    sprites = (GfxSprite **)self->base.data;
    sprites[0x20]->flags = sprites[0x20]->flags | 1;
    UiUnlockResultBeginPop(self, 0, 0x1f);
    UiUnlockResultBeginPop(self, 0, 0x20);
  } else {
    UiUnlockResultBeginPop(self, closing, 0x1f);
    UiUnlockResultBeginPop(self, closing, 0x20);
  }
}
