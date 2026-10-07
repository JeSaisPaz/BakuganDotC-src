// bdc 0x08989988 UiCollectionTheaterZoomCursorCell
#include "bdc.h"

/* Scales the cursor, the selected cell and its thumbnail up to 1.2 (+0.1 per frame of `zoom`) in
   `UiCollectionTheater` and brings them to the front. */

void UiCollectionTheaterZoomCursorCell(UiScreen *screen)
{
  UiCollectionTheater *self = (UiCollectionTheater *)screen;
  float t;
  float scale;

  t = self->zoom;
  if (t < 1.0f) {
    t = t + 0.1f;
    self->zoom = t;
  }
  scale = t * 0.20000005f + 1.0f;
  if (!(scale <= 1.2f)) {
    scale = 1.2f;
  }
  UiSpriteSetScaleRotation(((GfxSprite **)screen->data)[6], scale, scale, 0.0f);
  ((GfxSprite **)screen->data)[6]->posZ = -502.0f;
  UiSpriteSetScaleRotation(((GfxSprite **)screen->data)[self->cursor], scale, scale, 0.0f);
  ((GfxSprite **)screen->data)[self->cursor]->posZ = -500.0f;
  UiSpriteSetScaleRotation(((GfxSprite **)screen->data)[31 + self->cursor], scale, scale, 0.0f);
  ((GfxSprite **)screen->data)[31 + self->cursor]->posZ = -501.0f;
}
