// bdc 0x08971cdc UiOptionPrintHelp
#include "bdc.h"

/* Prints help message `index` of `UiOption` (table `"DOMesHelp"`,
   `SaveFindLocalizedBin`/`UiMesTableRelocate`) into `helpText` and lays it out in the help
   printer at (`x`, `y`) after clearing its sprites (printer vtable slot 2), measures it
   (`UiTextMeasure` → `helpWidth`/`helpHeight`/`helpLines`), records the glyph list and count in
   `helpGlyphs`/`helpGlyphCount` and sets `helpAlpha` to 1. */

void UiOptionPrintHelp(float x, float y, UiOption *self, u8 index)
{
  u32 *table;
  char *dst;
  UiTextPrinter *printer;
  const VtblEntry *entry;
  int count;

  table = SaveFindLocalizedBin("DOMesHelp");
  UiMesTableRelocate(table);
  dst = self->helpText;
  strcpy(dst, ((char **)table)[index]);
  printer = self->helpPrinter;
  GfxSpriteLayerClear(&printer->layer);
  printer->glyphs = NULL;

  printer = self->helpPrinter;
  entry = &printer->layer.vtbl[2];
  ((void (*)(float, float, float, void *, char *, s32, s32, s32))entry->fn)(
      x, y, 0.0f, (u8 *)printer + entry->delta, dst, 1, 0, 0);
  UiTextMeasure(0.0f, self->helpPrinter, dst, &self->helpWidth, &self->helpHeight,
                &self->helpLines);

  printer = self->helpPrinter;
  self->helpGlyphs = printer->glyphs;
  count = printer->glyphCount;
  self->helpAlpha = 1.0f;
  self->helpGlyphCount = (float)count;
}
