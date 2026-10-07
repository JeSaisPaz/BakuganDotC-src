// bdc 0x089a5ad0 UiHelpLineShow
#include "bdc.h"

/* Shows help message `msgIndex` of `DOMesHelp` (localized via `SaveFindLocalizedBin`) at screen
   position (`x`, `y`) in the shared help line: copies the text to `g_helpLineText`, clears the
   printer layer and re-prints it (printer vtable slot 2), measures it (`UiTextMeasure` into
   `g_helpLineWidth`/`g_helpLineHeight`/`g_helpLineLines`), records the glyph list head and
   count (`g_helpLineGlyphHead`, `g_helpLineGlyphCount`) and makes it fully visible
   (`g_helpLineAlpha` = 1). */

void UiHelpLineShow(float x, float y, u8 msgIndex)

{
  UiTextPrinter *printer;
  const VtblEntry *entry;
  u32 *table;

  table = SaveFindLocalizedBin("DOMesHelp");
  UiMesTableRelocate(table);
  strcpy(g_helpLineText, ((char **)table)[msgIndex]);
  printer = g_helpLinePrinter;
  GfxSpriteLayerClear(&printer->layer);
  printer->glyphs = NULL;
  printer = g_helpLinePrinter;
  entry = &printer->layer.vtbl[2];
  ((void (*)(float, float, float, void *, char *, s32, s32, s32))entry->fn)(
      x, y, 0.0f, (u8 *)printer + entry->delta, g_helpLineText, 1, 0, 0);
  UiTextMeasure(0.0f, g_helpLinePrinter, g_helpLineText, &g_helpLineWidth, &g_helpLineHeight,
                &g_helpLineLines);
  printer = g_helpLinePrinter;
  g_helpLineGlyphHead = printer->glyphs;
  g_helpLineAlpha = 1.0f;
  g_helpLineGlyphCount = (float)printer->glyphCount;
}
