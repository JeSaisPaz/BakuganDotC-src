// bdc 0x08954048 UiBattleRuleSelectAnimateSubFocus
#include "bdc.h"

/* Grows the focused sub-option of `UiBattleRuleSelect`: while
   `subFocusZoom` is below 1 it advances by 0.2, and the option button (0x0b/0x0e + `subCursor`),
   its label (0x11 + `subCursor`) and the cursor 0x15 are scaled to `1 + 0.02·zoom` (capped at
   1.02) and moved to depths -200/-201/-202. */

void UiBattleRuleSelectAnimateSubFocus(UiBattleRuleSelect *self)
{
  float zoom;
  float scale;
  int base;

  zoom = self->subFocusZoom;
  if (zoom < 1.0f) {
    zoom = zoom + 0.2f;
    self->subFocusZoom = zoom;
  }
  scale = zoom * 0.019999981f + 1.0f;
  if (!(scale <= 1.02f)) {
    scale = 1.02f;
  }
  base = 0xb;
  if (SaveGetProfileFlag0() != 0) {
    base = 0xe;
  }
  UiSpriteSetScaleRotation(((GfxSprite **)self->base.data)[base + self->subCursor], scale, scale, 0.0f);
  ((GfxSprite **)self->base.data)[base + self->subCursor]->posZ = -200.0f;
  UiSpriteSetScaleRotation(((GfxSprite **)self->base.data)[0x11 + self->subCursor], scale, scale, 0.0f);
  ((GfxSprite **)self->base.data)[0x11 + self->subCursor]->posZ = -201.0f;
  UiSpriteSetScaleRotation(((GfxSprite **)self->base.data)[0x15], scale, scale, 0.0f);
  ((GfxSprite **)self->base.data)[0x15]->posZ = -202.0f;
}
