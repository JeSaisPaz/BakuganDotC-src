// bdc 0x08919410 UiAdvSelectUpdateBlinkB
#include "bdc.h"

/* Same easing blink as `UiAdvSelectUpdateBlinkA` for sprite 0x19 (record `+0x8c4`). */

void UiAdvSelectUpdateBlinkB(UiAdvSelect *self)
{
  float t;
  GfxSprite *sprite;

  if (self->blinkBOn != 0) {
    t = self->blinkBT + 0.025f;
    self->blinkBT = t;
    sprite = ((GfxSprite **)(self->base).data)[25];
    if (self->blinkBPhase == 0) {
      sprite->alpha = self->blinkBBase - t * t * 0.8f;
      if (1.0f <= self->blinkBT) {
        ((GfxSprite **)(self->base).data)[25]->alpha = 0.2f;
        self->blinkBPhase = 1;
        self->blinkBT = 0.0f;
        self->blinkBBase = 0.2f;
        return;
      }
    } else {
      sprite->alpha = self->blinkBBase + (1.0f - (t - 1.0f) * (t - 1.0f)) * 0.8f;
      if (1.0f <= self->blinkBT) {
        ((GfxSprite **)(self->base).data)[25]->alpha = 1.0f;
        self->blinkBPhase = 0;
        self->blinkBT = 0.0f;
        self->blinkBBase = 1.0f;
      }
    }
  }
}
