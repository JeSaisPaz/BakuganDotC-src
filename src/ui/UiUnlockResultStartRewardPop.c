// bdc 0x08939c6c UiUnlockResultStartRewardPop
#include "bdc.h"

/* Starts the open/close animation of the reward itself on `UiUnlockResult`:
   for 2D rewards (reward kind `+0x5ee` ≠ 5/6) sets and shows the reward image on sprite 0x0c
   (`UiUnlockResultSetRewardTexture`, not for kinds 4/8/9) and begins its pop
   (`UiUnlockResultBeginPop`); for 3D rewards begins the model fade
   (`UiUnlockResultBeginModelFade`). */

void UiUnlockResultStartRewardPop(UiUnlockResult *self, u8 closing)
{
  GfxSprite **sprites;
  u8 kind = self->rewardKind;

  if (kind != 5 && kind != 6) {
    if (closing == 0) {
      if (kind != 4 && kind != 8 && kind != 9) {
        UiUnlockResultSetRewardTexture(self, ((GfxSprite **)self->base.data)[12]);
        sprites = (GfxSprite **)self->base.data;
        sprites[12]->flags = sprites[12]->flags | 1;
      }
      UiUnlockResultBeginPop(self, 0, 0xc);
    } else {
      UiUnlockResultBeginPop(self, closing, 0xc);
    }
  } else {
    UiUnlockResultBeginModelFade(self, closing);
  }
}
