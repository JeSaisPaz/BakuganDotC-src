// bdc 0x08933a0c UiGauntletSetupTweenFrameDone
#include "bdc.h"

/* Advances the frame tweens started by `UiGauntletSetupTweenFrame` (sprites 0x1c–0x21 with tweens
   0x1c–0x21 at `+0x4d8`, then sprite 0 with tween 0 at `+0x78`) with `UiTweenUpdate` (scale 1 → 1,
   16 frames, flags 1; `closing` selects fade-out) and returns true when any of them reports its
   transition finished (the u8 sum of the results is non-zero; they run in lockstep). */

bool UiGauntletSetupTweenFrameDone(UiGauntletSetup *self, u8 closing)
{
  GfxSprite **sprites = (GfxSprite **)self->base.data;
  u8 finished = 0;
  int i;

  for (i = 0x1c; i < 0x22; i++) {
    finished += UiTweenUpdate(1.0f, 1.0f, 16.0f, closing, sprites[i], &self->tweens[i], 1);
  }
  for (i = 0; i < 1; i++) {
    finished += UiTweenUpdate(1.0f, 1.0f, 16.0f, closing, sprites[i], &self->tweens[i], 1);
  }
  return finished != 0;
}
