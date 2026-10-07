// bdc 0x088c9758 UiTalkBalloonAnimatePortrait
#include "bdc.h"

/* Lip-sync style portrait animation of the talk balloon (`UiTalkBalloonCtor`): every 3 frames
   swaps the portrait sprite's texture between `"cha_%02d_1"` and `"cha_%02d_2"` for `speaker`
   (none when −1). */

void UiTalkBalloonAnimatePortrait(UiTalkBalloon *self)
{
  char name[64];
  int frame;
  GfxSprite *sprite;

  if (-1 < self->speaker) {
    frame = self->portraitFrame + 1;
    self->portraitFrame = frame;
    if (frame % 3 == 0) {
      if (((frame / 3) & 1U) == 0) {
        sprintf(name, "cha_%02d_%d", self->speaker + 1, 1);
        sprite = self->parts[2];
      } else {
        sprintf(name, "cha_%02d_%d", self->speaker + 1, 2);
        sprite = self->parts[2];
      }
      sprite->texture = GfxFindTexture(name);
    }
  }
}
