// bdc 0x089192c8 UiAdvSelectUpdateBlinkA
#include "bdc.h"

/* Animates the alpha of sprite 0x25 of the adventure partner-select screen (`UiAdvSelectCtor`,
   task 376; cursor `+0x74`, candidates `+0x8a0` as 4-byte `{id, partner, locked, ?}` slots) while
   the record `+0x8b8` is on: eases from the base alpha down to 0.2 (`base - 0.8 t^2`) then back up
   to 1.0, `t` advancing by 0.025 per frame. */

void UiAdvSelectUpdateBlinkA(UiAdvSelect *self)
{
  float t;
  GfxSprite *sprite;

  if (self->blinkAOn != 0) {
    t = self->blinkAT + 0.025f;
    self->blinkAT = t;
    sprite = ((GfxSprite **)(self->base).data)[37];
    if (self->blinkAPhase == 0) {
      sprite->alpha = self->blinkABase - t * t * 0.8f;
      if (1.0f <= self->blinkAT) {
        ((GfxSprite **)(self->base).data)[37]->alpha = 0.2f;
        self->blinkAPhase = 1;
        self->blinkAT = 0.0f;
        self->blinkABase = 0.2f;
        return;
      }
    } else {
      sprite->alpha = self->blinkABase + (1.0f - (t - 1.0f) * (t - 1.0f)) * 0.8f;
      if (1.0f <= self->blinkAT) {
        ((GfxSprite **)(self->base).data)[37]->alpha = 1.0f;
        self->blinkAPhase = 0;
        self->blinkAT = 0.0f;
        self->blinkABase = 1.0f;
      }
    }
  }
}
