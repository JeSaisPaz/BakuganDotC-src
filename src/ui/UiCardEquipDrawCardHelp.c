// bdc 0x08969870 UiCardEquipDrawCardHelp
#include "bdc.h"

/* Draws the card-help text printer of `UiCardEquip` at depth 4000; same as `UiCardEquipDrawCardName` but for the help fields and g_colorWhite. */

void UiCardEquipDrawCardHelp(UiCardEquip *self)
{
  UiTextPrinter *printer;
  GfxSprite *sprite;
  void *packet;
  float *outline;
  s32 i;

  if (self->helpPrinter != NULL) {
    if (self->helpGlyphCount != 0.0f && self->helpDirty != 0) {
      sprite = self->helpGlyphs;
      i = 0;
      if (0.0f < self->helpGlyphCount) {
        do {
          i++;
          sprite->alpha = self->helpAlpha;
          sprite = sprite->next;
        } while ((float)i < self->helpGlyphCount);
      }
      self->helpDirty = 0;
    }
    /* copies g_colorWhite into the printer outline colour (lv.q/sv.q) */
    outline = self->helpPrinter->outlineColor;
    outline[0] = g_colorWhite.x;
    outline[1] = g_colorWhite.y;
    outline[2] = g_colorWhite.z;
    outline[3] = g_colorWhite.w;
    printer = self->helpPrinter;
    packet = GfxNewRenderPacket(4000.0f);
    GfxSpriteLayerDraw(&printer->layer, packet);
  }
}
