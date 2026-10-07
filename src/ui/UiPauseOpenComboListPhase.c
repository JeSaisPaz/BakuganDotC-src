// bdc 0x08910a70 UiPauseOpenComboListPhase
#include "bdc.h"

/* Phase 4 of the pause menu: zooms/fades the menu out over 6 frames (layer alpha `-0.2`, scale
   `*1.1`), then creates the combo list screen (task 3002, `UiComboListCtor`) and switches to
   phase 6. */

void UiPauseOpenComboListPhase(UiPause *self)

{
  int t;
  GfxSpriteLayer *layer;

  layer = self->base.spriteLayer;
  layer->alpha = layer->alpha - 0.2f;
  self->scaleX = self->scaleX * 1.1f;
  t = self->closeTimer;
  self->closeTimer = t + 1;
  if (t >= 6) {
    self->base.spriteLayer->alpha = 0.0f;
    CoreTaskCreate(0xbba,100);
    (self->base).phase = 6;
  }
  return;
}

