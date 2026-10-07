// bdc 0x0881846c UiFontGlyphAdvance
#include "bdc.h"

/* Horizontal advance of glyph `glyph` in the printer's current font: with a per-glyph width table
   (`widthTable`) it is `(width[glyph] + spacing) * widthScale`, otherwise the fixed cell width
   `advanceX` times `scale`. */

float UiFontGlyphAdvance(void *printer, int glyph)

{
  struct UiTextPrinter *p = (struct UiTextPrinter *)printer;
  if (p->widthTable != 0) {
    return (((const float *)p->widthTable)[glyph] + p->spacing) * p->widthScale;
  }
  return p->advanceX * p->scale;
}
