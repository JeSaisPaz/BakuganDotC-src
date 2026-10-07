// bdc 0x08910afc UiPauseReturnPhase
#include "bdc.h"

/* Phase 5 of the pause menu: eases the menu back in (layer alpha `+0.2`, scale towards 1) after a
   sub-screen and returns to phase 2 (main) with frame mode 1. */

void UiPauseReturnPhase(UiPause *self)

{
  GfxSpriteLayer *layer;
  float delta;

  layer = (self->base).spriteLayer;
  layer->alpha = layer->alpha + 0.2f;
  delta = (1.0f - self->scaleX) * 0.6f;
  self->scaleX = self->scaleX + (delta - 0.01f);
  if (ABS(delta) < 0.02f) {
    ((self->base).spriteLayer)->alpha = 1.0f;
    self->scaleX = 1.0f;
    (self->base).phase = 2;
    UiScreenSetFrameMode((CoreTask *)self,1);
  }
  return;
}
