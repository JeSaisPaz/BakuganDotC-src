// bdc 0x0893ca68 UiUnlockResultLayoutFrame
#include "bdc.h"

/* Re-positions the six frame/decoration sprites (data sprites 6..11) of the unlock-result screen
   (task 375, `UiUnlockResultCtor`) around the main frame sprite (data sprite 12): each corner is
   the frame position plus/minus the frame scale times its `cornerOffset` factor. Skipped for
   reward kinds 5 and 6. */

void UiUnlockResultLayoutFrame(UiUnlockResult *self)

{
  GfxSprite **sprites;
  GfxSprite *frame;

  if (self->rewardKind == 5 || self->rewardKind == 6) {
    return;
  }
  sprites = (GfxSprite **)self->base.data;
  frame = sprites[12];
  sprites[6]->posX = frame->posX - frame->scaleX * self->cornerOffset[0][0];
  sprites = (GfxSprite **)self->base.data;
  frame = sprites[12];
  sprites[6]->posY = frame->posY - frame->scaleY * self->cornerOffset[0][1];
  sprites = (GfxSprite **)self->base.data;
  frame = sprites[12];
  sprites[7]->posX = frame->posX + frame->scaleX * self->cornerOffset[1][0];
  sprites = (GfxSprite **)self->base.data;
  frame = sprites[12];
  sprites[7]->posY = frame->posY + frame->scaleY * self->cornerOffset[1][1];
  sprites = (GfxSprite **)self->base.data;
  frame = sprites[12];
  sprites[8]->posX = frame->posX + frame->scaleX * self->cornerOffset[2][0];
  sprites = (GfxSprite **)self->base.data;
  frame = sprites[12];
  sprites[8]->posY = frame->posY + frame->scaleY * self->cornerOffset[2][1];
  sprites = (GfxSprite **)self->base.data;
  frame = sprites[12];
  sprites[9]->posX = frame->posX + frame->scaleX * self->cornerOffset[3][0];
  sprites = (GfxSprite **)self->base.data;
  frame = sprites[12];
  sprites[9]->posY = frame->posY + frame->scaleY * self->cornerOffset[3][1];
  sprites = (GfxSprite **)self->base.data;
  frame = sprites[12];
  sprites[10]->posX = frame->posX - frame->scaleX * self->cornerOffset[4][0];
  sprites = (GfxSprite **)self->base.data;
  frame = sprites[12];
  sprites[10]->posY = frame->posY + frame->scaleY * self->cornerOffset[4][1];
  sprites = (GfxSprite **)self->base.data;
  frame = sprites[12];
  sprites[11]->posX = frame->posX - frame->scaleX * self->cornerOffset[5][0];
  sprites = (GfxSprite **)self->base.data;
  frame = sprites[12];
  sprites[11]->posY = frame->posY + frame->scaleY * self->cornerOffset[5][1];
}
