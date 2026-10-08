// bdc 0x08934a58 UiGauntletSetupSetCardName
#include "bdc.h"

/* Prints the name of card `card` into the card-name box of `UiGauntletSetup`:
   copies entry `card` of the `"DWCardName"` text table (`SaveFindLocalizedBin`) to `nameText`,
   clears the printer `namePrinter` and prints the text 34 px above sprite 0x36 (printer vtable
   slot 2), measures it (`UiTextMeasure` into `nameWidth`/`nameHeight`/`nameLines`), records the
   glyph list `nameGlyphs` and its count in `nameReveal`, saves each glyph's X in `nameGlyphX` and
   resets the box alpha (`nameAlpha` = 0, `nameAppliedAlpha` = 1). */

void UiGauntletSetupSetCardName(UiGauntletSetup *self, u8 card)
{
  u32 *table;
  char *dst;
  UiTextPrinter *printer;
  GfxSprite *anchor;
  const VtblEntry *entry;
  GfxSprite *glyph;
  float count;
  int i;

  table = SaveFindLocalizedBin("DWCardName");
  UiMesTableRelocate(table);
  dst = self->nameText;
  strcpy(dst, (const char *)PspPtr(table[card]));
  printer = self->namePrinter;
  GfxSpriteLayerClear(&printer->layer);
  printer->glyphs = NULL;

  printer = self->namePrinter;
  anchor = ((GfxSprite **)self->base.data)[0x36];
  entry = &printer->layer.vtbl[2];
  ((void (*)(float, float, float, void *, char *, s32, s32, s32))entry->fn)(
      anchor->posX, anchor->posY - 34.0f, 0.0f, (u8 *)printer + entry->delta, dst, 1, 0, 0);
  UiTextMeasure(0.0f, self->namePrinter, dst, &self->nameWidth, &self->nameHeight, &self->nameLines);

  printer = self->namePrinter;
  self->nameGlyphs = printer->glyphs;
  count = (float)printer->glyphCount;
  glyph = self->nameGlyphs;
  i = 0;
  self->nameReveal = count;
  if (0.0f < count) {
    do {
      self->nameGlyphX[i] = glyph->posX;
      i++;
      glyph = glyph->next;
    } while ((float)i < count);
  }
  self->nameAlpha = 0.0f;
  self->nameAppliedAlpha = 1.0f;
}
