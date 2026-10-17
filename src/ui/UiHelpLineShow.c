// bdc 0x089a5ad0 UiHelpLineShow
#include "bdc.h"

/* Shows help message `msgIndex` of `DOMesHelp` (localized via `SaveFindLocalizedBin`) at screen
   position (`x`, `y`) in the shared help line `g_helpLine`: copies the text to its `text`,
   clears the printer layer and re-prints it (printer vtable slot 2), measures it
   (`UiTextMeasure` into `height`, `width` (widest line) and `textLen`), records the glyph list head and count
   (`glyphHead`, `glyphCount`) and makes it fully visible (`alpha` = 1). */

void UiHelpLineShow(float x, float y, u8 msgIndex)

{
  UiTextPrinter *printer;
  const VtblEntry *entry;
  u32 *table;

  table = SaveFindLocalizedBin("DOMesHelp");
  UiMesTableRelocate(table);
  strcpy(g_helpLine.text, (const char *)PspPtr(table[msgIndex]));
  printer = g_helpLine.printer;
  GfxSpriteLayerClear(&printer->layer);
  printer->glyphs = NULL;
  printer = g_helpLine.printer;
  entry = &printer->layer.vtbl[2];
  ((void (*)(float, float, float, void *, char *, u8, s32, u8))entry->fn)(
      x, y, 0.0f, (u8 *)printer + entry->delta, g_helpLine.text, 1, 0, 0);
  UiTextMeasure(0.0f, g_helpLine.printer, g_helpLine.text, &g_helpLine.height, &g_helpLine.width,
                &g_helpLine.textLen);
  printer = g_helpLine.printer;
  g_helpLine.glyphHead = printer->glyphs;
  g_helpLine.alpha = 1.0f;
  g_helpLine.glyphCount = (float)printer->glyphCount;
}
