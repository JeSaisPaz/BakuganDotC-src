// bdc 0x08817a50 UiTextPrinterSetFont
#include "bdc.h"

/* Selects font `font` for the printer: for `font < 4` loads its texture list
   (`UiTextPrinterLoadFonts` with `g_uiFontTextureLists``[font]`, a negative id is not
   rejected) and, if that returns non-zero, sets the matching metrics: font 0 cell 14x14,
   advance 12, line 13, spacing 2, widths `g_uiFont0Widths`, scale 1; font 1 cell/advance/line 16,
   spacing 3, widths `g_uiFont12Widths`, scale 2/3; font 2 the same at 24 with scale 1; font 3
   24 with `g_uiFont3Widths`, scale 1. Other ids change nothing.
   Returns the `UiTextPrinterLoadFonts` result (0 when `font >= 4` or the load failed). */

s32 UiTextPrinterSetFont(UiTextPrinter *self, int font)
{
  s32 ok;

  ok = 0;
  if (font < 4) {
    ok = UiTextPrinterLoadFonts(self, (const char **)g_uiFontTextureLists[font]);
  }
  if (ok == 0) {
    return 0;
  }
  switch (font) {
  case 0:
    self->cellW = 14.0f;
    self->cellH = 14.0f;
    self->advanceX = 12.0f;
    self->lineHeight = 13.0f;
    self->spacing = 2.0f;
    self->widthTable = g_uiFont0Widths;
    self->widthScale = 1.0f;
    break;
  case 1:
    self->cellW = 16.0f;
    self->cellH = 16.0f;
    self->advanceX = 16.0f;
    self->lineHeight = 16.0f;
    self->spacing = 3.0f;
    self->widthTable = g_uiFont12Widths;
    self->widthScale = 0.6666667f;
    break;
  case 2:
    self->cellW = 24.0f;
    self->cellH = 24.0f;
    self->advanceX = 24.0f;
    self->lineHeight = 24.0f;
    self->spacing = 3.0f;
    self->widthTable = g_uiFont12Widths;
    self->widthScale = 1.0f;
    break;
  case 3:
    self->cellW = 24.0f;
    self->cellH = 24.0f;
    self->advanceX = 24.0f;
    self->lineHeight = 24.0f;
    self->spacing = 3.0f;
    self->widthTable = g_uiFont3Widths;
    self->widthScale = 1.0f;
    break;
  }
  return ok;
}
