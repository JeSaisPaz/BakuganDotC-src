// bdc 0x0897151c UiOptionZoomButton
#include "bdc.h"

/* When a button row (4 OK, 5 Defaults) of `UiOption` is selected, scales its button
   (layout sprite 44 + row), label (46 + row) and the glow sprite (52) up to 1.2 (`zoomFrame` +0.1 per
   frame up to 1, scale 1 + 0.2 * zoomFrame) and brings them to the front (Z -1000/-1001/-1002). */

void UiOptionZoomButton(UiOption *self)
{
  int idx;
  float zoom;
  float scale;

  if ((s8)self->cursor >= 4) {
    zoom = self->zoomFrame;
    if (zoom < 1.0f) {
      zoom = zoom + 0.1f;
      self->zoomFrame = zoom;
    }
    scale = zoom * 0.20000005f + 1.0f;
    if (!(scale <= 1.2f)) {
      scale = 1.2f;
    }
    idx = (s8)self->cursor + 0x2c;
    UiSpriteSetScaleRotation(((GfxSprite **)self->base.data)[idx], scale, scale, 0.0f);
    ((GfxSprite **)self->base.data)[idx]->posZ = -1000.0f;
    idx = (s8)self->cursor + 0x2e;
    UiSpriteSetScaleRotation(((GfxSprite **)self->base.data)[idx], scale, scale, 0.0f);
    ((GfxSprite **)self->base.data)[idx]->posZ = -1001.0f;
    if ((s8)self->cursor >= 4) {
      UiSpriteSetScaleRotation(((GfxSprite **)self->base.data)[0x34], scale, scale, 0.0f);
      ((GfxSprite **)self->base.data)[0x34]->posZ = -1002.0f;
    }
  }
}
