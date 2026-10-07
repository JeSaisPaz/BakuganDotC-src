// bdc 0x0893af78 UiUnlockResultBlinkSprite05
#include "bdc.h"

/* Per-frame blink of sprite 5 of `UiUnlockResult`: over 30 frames fades its
   alpha from 1 to 0.5, then back to 1, alternating (`blinkT`, `blinkBase` start alpha,
   `blinkPhase` direction). */

void UiUnlockResultBlinkSprite05(UiUnlockResult *self)
{
  GfxSprite *sprite;
  float half;

  self->blinkT = self->blinkT + 0.033333335f;
  half = self->blinkT * 0.5f;
  sprite = ((GfxSprite **)self->base.data)[5];
  if (self->blinkPhase == '\0') {
    sprite->alpha = self->blinkBase - half;
    if (!(self->blinkT < 1.0f)) {
      ((GfxSprite **)self->base.data)[5]->alpha = 0.5f;
      half = ((GfxSprite **)self->base.data)[5]->alpha;
      self->blinkT = 0.0f;
      self->blinkBase = half;
      self->blinkPhase = 1;
    }
  } else {
    sprite->alpha = self->blinkBase + half;
    if (!(self->blinkT < 1.0f)) {
      ((GfxSprite **)self->base.data)[5]->alpha = 1.0f;
      half = ((GfxSprite **)self->base.data)[5]->alpha;
      self->blinkT = 0.0f;
      self->blinkBase = half;
      self->blinkPhase = 0;
    }
  }
}
