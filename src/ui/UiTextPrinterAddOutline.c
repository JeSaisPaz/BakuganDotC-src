// bdc 0x08818944 UiTextPrinterAddOutline
#include "bdc.h"

/* Text printer vtable slot 4 (`0x08af1654`): gives the glyph sprites just printed (`glyphs`,
   `glyphCount`) a 1-pixel outline. For each of the 8 offsets in `g_uiOutlineOffsets` it clones
   every glyph (`GfxSpriteLayerCloneSprite`); in the first 7 passes the clone gets the outline
   colour (`outlineColor` -> `tint`/`alpha`) and is shifted by the offset and +1 in depth, in the 8th
   pass (offset 0,0) the same is applied to the original glyph instead (the clone keeps the glyph's
   look). A pass stops at the end of the list or when a clone fails. */

void UiTextPrinterAddOutline(UiTextPrinter *self)
{
  int pass;
  int i;
  GfxSprite *src;
  GfxSprite *clone;
  GfxSprite *dst;

  for (pass = 0; pass < 8; pass++) {
    src = self->glyphs;
    for (i = 0; i < self->glyphCount; i++) {
      if (src == NULL) {
        break;
      }
      clone = GfxSpriteLayerCloneSprite(&self->layer, src);
      if (clone == NULL) {
        break;
      }
      dst = (pass < 7) ? clone : src;
      dst->tint[0] = self->outlineColor[0];
      dst->tint[1] = self->outlineColor[1];
      dst->tint[2] = self->outlineColor[2];
      dst->alpha = self->outlineColor[3];
      dst->posX = dst->posX + (float)g_uiOutlineOffsets[pass][0];
      dst->posY = dst->posY + (float)g_uiOutlineOffsets[pass][1];
      dst->posZ = dst->posZ + 1.0f;
      src = src->next;
    }
  }
}
