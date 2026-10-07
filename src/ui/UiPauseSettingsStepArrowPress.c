// bdc 0x089aee8c UiPauseSettingsStepArrowPress
#include "bdc.h"

/* Animates the pressed arrow of `UiPauseSettings` (the three sprites
   `data[1 + cursor*3 + dir*12 + i]`, i = 0..2, row `cursor` `+0x74`, direction `dir` `+0xb7a`)
   while the UI flash slot 0 runs (`UiFlashStep`): tints the first sprite 0.3 grey (alpha 1) and
   grows all three by 0.1 per frame (scaleY follows scaleX, angle 0), returning false; once the
   flash has ended restores white tint, alpha 1 and scale 1.0 and returns true. */

/* The asm re-reads cursor, dir and data for every access. */
static inline GfxSprite *ArrowSprite(UiPauseSettings *self, int i)
{
  return ((GfxSprite **)self->base.data)[i + self->cursor * 3 + self->dir * 12 + 1];
}

bool UiPauseSettingsStepArrowPress(UiPauseSettings *self)
{
  int done;
  GfxSprite *first;
  GfxSprite *sprite;
  int i;

  done = UiFlashStep(0);
  first = ArrowSprite(self, 0);
  if (done != 0) {
    first->tint[0] = 1.0f;
    first->tint[1] = 1.0f;
    first->tint[2] = 1.0f;
    first->alpha = 1.0f;
    for (i = 0; i < 3; i++) {
      ArrowSprite(self, i)->scaleX = 1.0f;
      sprite = ArrowSprite(self, i);
      sprite->scaleY = sprite->scaleX;
      ArrowSprite(self, i)->angle = 0.0f;
      sprite = ArrowSprite(self, i);
      GfxSpriteSetScaleRotation(sprite, sprite->scaleX, sprite->scaleY, sprite->angle, false);
    }
    return true;
  }
  first->alpha = 1.0f;
  first->tint[0] = 0.3f;
  first->tint[1] = 0.3f;
  first->tint[2] = 0.3f;
  for (i = 0; i < 3; i++) {
    sprite = ArrowSprite(self, i);
    sprite->scaleX = sprite->scaleX + 0.1f;
    sprite = ArrowSprite(self, i);
    sprite->scaleY = sprite->scaleX;
    ArrowSprite(self, i)->angle = 0.0f;
    sprite = ArrowSprite(self, i);
    GfxSpriteSetScaleRotation(sprite, sprite->scaleX, sprite->scaleY, sprite->angle, false);
  }
  return false;
}
