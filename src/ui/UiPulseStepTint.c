// bdc 0x089a533c UiPulseStepTint
#include "bdc.h"

/* Oscillates a pulse record's tint level (`+0x20`, direction byte `+7`) between 0 and 0.3 with step
   `0.3/frames` and applies `0.3 - level` as RGB colour-add to `sprite` (a breathing glow on the
   cursor). */

void UiPulseStepTint(float frames, GfxSprite *sprite, UiPulse *self)
{
  float level;
  float tint;

  if (self->waiting == '\0') {
    level = self->level + 0.3f / frames;
    self->level = level;
    if (!(level < 0.3f)) {
      self->level = 0.3f;
      level = 0.3f;
      self->waiting = '\x01';
    }
  }
  else {
    level = self->level - 0.3f / frames;
    self->level = level;
    if (level <= 0.0f) {
      self->level = 0.0f;
      level = 0.0f;
      self->waiting = '\0';
    }
  }
  tint = 0.3f - level;
  sprite->addColor[3] = 1.0f;
  sprite->addColor[0] = tint;
  sprite->addColor[1] = tint;
  sprite->addColor[2] = tint;
}
