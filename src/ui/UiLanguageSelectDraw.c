// bdc 0x08809ed8 UiLanguageSelectDraw
#include "bdc.h"

/* Draw method of the language-selection screen (id 199): in state 2 (and in state 3 while the
   sub-step `+0x14 > 0`) draws the sprite pool `+0x1c` into a depth-50 render packet
   (`GfxSpriteLayerDraw`). */

void UiLanguageSelectDraw(CoreTask *task)

{
  UiLanguageSelect *self = (UiLanguageSelect *)task;
  bool draw = false;
  void *packet;

  if (self->state < 3) {
    if (self->state >= 2) {
      draw = true;
    }
  } else if (self->state < 4 && self->subStep > 0) {
    draw = true;
  }
  if (draw) {
    packet = GfxNewRenderPacket(50.0f);
    if (self->layer != NULL) {
      GfxSpriteLayerDraw(self->layer, packet);
    }
  }
}
