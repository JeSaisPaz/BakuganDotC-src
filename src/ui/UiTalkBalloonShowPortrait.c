// bdc 0x088cc404 UiTalkBalloonShowPortrait
#include "bdc.h"

/* Shows the portrait of `speaker` in the talk balloon (`UiTalkBalloonCtor`): texture
   `"cha_%02d_1"` on the portrait sprite, makes it and its frame visible and resets the lip-flap
   counter `portraitFrame`. Does nothing when there is no speaker (−1). */

void UiTalkBalloonShowPortrait(UiTalkBalloon *self)
{
  char name[64];

  if (-1 < self->speaker) {
    sprintf(name, "cha_%02d_%d", self->speaker + 1, 1);
    GfxSprite *portrait = self->parts[2];
    portrait->texture = GfxFindTexture(name);
    self->parts[3]->flags = self->parts[3]->flags | 1;
    self->parts[2]->flags = self->parts[2]->flags | 1;
    self->portraitFrame = 0;
  }
}
