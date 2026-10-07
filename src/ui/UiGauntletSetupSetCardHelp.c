// bdc 0x08934bb4 UiGauntletSetupSetCardHelp
#include "bdc.h"

/* Sets the description text of card `card` on the gauntlet setup screen: copies entry `card` of
   the `"DWCardHelp"` message table into `helpText`, clears the help printer `helpPrinter` and
   prints the text at the position of sprite 0x36 (printer vtable slot 2), measures it
   (`UiTextMeasure` into `helpWidth`/`helpHeight`/`helpLines`), then centres the glyphs
   vertically (moves every glyph up by half the largest Y offset from the first glyph), records
   each glyph's X in `helpGlyphX`, and resets `helpReveal` = glyph count, `helpAlpha` = 0,
   `helpAppliedAlpha` = 1. */

void UiGauntletSetupSetCardHelp(UiGauntletSetup *self, u8 card)
{
  u32 *table;
  char *dst;
  UiTextPrinter *printer;
  const VtblEntry *entry;
  GfxSprite *anchor;
  GfxSprite *first;
  GfxSprite *glyph;
  float count;
  int maxDy;
  int dy;
  int i;

  table = SaveFindLocalizedBin("DWCardHelp");
  UiMesTableRelocate(table);
  dst = self->helpText;
  strcpy(dst, (const char *)(uintptr_t)table[card]);
  printer = self->helpPrinter;
  GfxSpriteLayerClear(&printer->layer);
  printer->glyphs = NULL;
  printer = self->helpPrinter;
  anchor = ((GfxSprite **)self->base.data)[0x36];
  entry = &printer->layer.vtbl[2];
  ((void (*)(float, float, float, void *, char *, s32, s32, s32))entry->fn)(
      anchor->posX, anchor->posY, 0.0f, (u8 *)printer + entry->delta, dst, 1, 0, 0);
  UiTextMeasure(0.0f, self->helpPrinter, dst, &self->helpWidth, &self->helpHeight,
                &self->helpLines);
  self->helpGlyphs = self->helpPrinter->glyphs;
  count = (float)self->helpPrinter->glyphCount;
  first = self->helpGlyphs;
  maxDy = 0;
  self->helpReveal = count;

  /* Largest vertical offset of any glyph from the first one. */
  if (0.0f < count) {
    i = 0;
    glyph = first;
    do {
      dy = (int)(glyph->posY - first->posY);
      if (maxDy < dy)
        maxDy = dy;
      i++;
      glyph = glyph->next;
    } while ((float)i < count);
  }

  /* Shift every glyph up by half of it (helpReveal re-read each iteration). */
  if (0.0f < count) {
    i = 0;
    glyph = first;
    do {
      i++;
      glyph->posY = glyph->posY - (float)(maxDy / 2);
      count = self->helpReveal;
      glyph = glyph->next;
    } while ((float)i < count);
    first = self->helpGlyphs;
  }

  /* Remember each glyph's X position. */
  if (0.0f < count) {
    i = 0;
    glyph = first;
    do {
      self->helpGlyphX[i] = glyph->posX;
      i++;
      glyph = glyph->next;
    } while ((float)i < count);
  }

  self->helpAlpha = 0.0f;
  self->helpAppliedAlpha = 1.0f;
}
