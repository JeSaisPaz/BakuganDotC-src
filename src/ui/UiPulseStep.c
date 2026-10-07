// bdc 0x089a5828 UiPulseStep
#include "bdc.h"

/* Steps a cursor pulse while `ghost` is visible: grows the ghost from the source's scale toward
   `targetScale` while fading it out over 30 frames (15 when frame skipping), then counts 8 wait
   frames; after those it restarts only once the source's tint (`addColor[0]`) is at least 0.3. */

void UiPulseStep(GfxSprite *ghost, UiPulse *self)
{
  s8 waiting;
  float frames;
  float phase;
  float t;
  float scaleX;

  if ((ghost->flags & 1) == 0) {
    return;
  }
  waiting = (s8)self->waiting;
  if (g_gfxDisplay->frameSkip != 0) {
    frames = 15.0f;
  } else {
    frames = 30.0f;
  }
  if (waiting != 0) {
    if ((s8)self->waitFrames != 8) {
      self->waitFrames = self->waitFrames + 1;
      return;
    }
  }
  if (!(self->source->addColor[0] < 0.3f)) {
    self->waiting = 0;
    waiting = (s8)self->waiting;
  }
  if (waiting != 0) {
    return;
  }

  phase = self->phase + 1.0f / frames;
  t = phase - 1.0f;
  self->phase = phase;
  ghost->alpha = self->level - (1.0f - t * t);

  t = self->phase - 1.0f;
  scaleX = self->source->scaleX + (1.0f - t * t) * (self->targetScale - 1.0f);
  ghost->scaleX = scaleX;
  GfxSpriteSetScaleRotation(ghost, scaleX, self->source->scaleY, ghost->angle, false);
  ghost->posZ = self->source->posZ + 1.0f;

  if (!(self->phase < 1.0f)) {
    self->phase = 0.0f;
    self->waiting = 1;
    self->waitFrames = 0;
    ghost->alpha = 0.0f;
  }
}
