// bdc 0x089b4338 UiComboListExitPhase
#include "bdc.h"

/* Phase 3 of the combo list screen: zooms and fades the layer out over 6 frames (alpha `-0.2f`,
   zoom `*1.1f`), then advances the phase and resets the animation counters. */

void UiComboListExitPhase(UiComboList *self)
{
  GfxSpriteLayer *layer;
  int frame;

  layer = self->base.spriteLayer;
  layer->alpha = layer->alpha - 0.2f;
  self->zoom = self->zoom * 1.1f;
  frame = self->animFrame;
  self->animFrame = frame + 1;
  if (frame >= 6) {
    self->animFrame = 0;
    self->step = 0;
    self->base.phase = self->base.phase + 1;
  }
}
