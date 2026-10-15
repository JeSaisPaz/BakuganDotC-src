// bdc 0x0881846c UiFontGlyphAdvance
#include "bdc.h"

/* Horizontal advance of glyph `glyph` in the printer's current font: with a per-glyph width table
   (`widthTable`) it is `(width[glyph] + spacing) * widthScale`, otherwise the fixed cell width
   `advanceX` times `scale`. */

/* `width[glyph]` as the PSP reads it. The font width tables `g_uiFont12Widths`, `g_uiFont3Widths`
   and `g_uiFont0Widths` are 523 floats each and lie back to back in that order, and the original
   does not bound the index: a glyph past a table (the two-byte codes of `UiFontNextGlyph` reach
   past 522) reads the start of the next table. Past `g_uiFont0Widths`, or past any other table,
   the following data is not modelled and the width reads 0. */
static float UiFontWidthAt(const float *table, int glyph)
{
  static const float *const run[3] = {g_uiFont12Widths, g_uiFont3Widths, g_uiFont0Widths};
  const int n = (int)(sizeof(g_uiFont0Widths) / sizeof(g_uiFont0Widths[0]));
  int i;

  for (i = 0; i < 3; i++) {
    if (table == run[i]) {
      if (glyph >= 0 && glyph < n * (3 - i)) {
        return run[i + BDC_DIV(glyph, n)][BDC_MOD(glyph, n)];
      }
      return 0.0f;
    }
  }
  return table[glyph];
}

float UiFontGlyphAdvance(void *printer, int glyph)

{
  struct UiTextPrinter *p = (struct UiTextPrinter *)printer;
  if (p->widthTable != 0) {
    return (UiFontWidthAt((const float *)p->widthTable, glyph) + p->spacing) * p->widthScale;
  }
  return p->advanceX * p->scale;
}
