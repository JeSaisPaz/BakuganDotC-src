// bdc 0x08933604 UiGauntletSetupFrameDone
#include "bdc.h"

/* Advances the frame tweens of the gauntlet setup screen by one frame: sprites 1..5 through the
   pulse helper `UiSpriteEaseStep` (16 frames, alpha step 0.3, flags 3) and sprite 6 through
   `UiTweenUpdate` (flags 0xb). Showing (`hide == 0`) scales 1.5 -> 1.0, hiding 1.0 -> 1.5.
   Returns true when the 8-bit count of tweens that reported "finished" this frame is non-zero,
   i.e. when at least one of them has finished. */

bool UiGauntletSetupFrameDone(UiGauntletSetup *self, u8 hide)
{
  GfxSprite **sprites;
  u8 finished;
  int i;

  finished = 0;
  if (hide == 0) {
    for (i = 1; i < 6; i++) {
      sprites = (GfxSprite **)self->base.data;
      finished += UiSpriteEaseStep(1.5f, 1.0f, 16.0f, 0.3f, hide, sprites[i], &self->tweens[i], 3);
    }
    for (i = 6; i < 7; i++) {
      sprites = (GfxSprite **)self->base.data;
      finished += UiTweenUpdate(1.5f, 1.0f, 16.0f, hide, sprites[i], &self->tweens[i], 0xb);
    }
  } else {
    for (i = 1; i < 6; i++) {
      sprites = (GfxSprite **)self->base.data;
      finished += UiSpriteEaseStep(1.0f, 1.5f, 16.0f, 0.3f, hide, sprites[i], &self->tweens[i], 3);
    }
    for (i = 6; i < 7; i++) {
      sprites = (GfxSprite **)self->base.data;
      finished += UiTweenUpdate(1.0f, 1.5f, 16.0f, hide, sprites[i], &self->tweens[i], 0xb);
    }
  }
  return finished != 0;
}
