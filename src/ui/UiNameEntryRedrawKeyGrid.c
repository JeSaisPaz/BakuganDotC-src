// bdc 0x08805ab4 UiNameEntryRedrawKeyGrid
#include "bdc.h"

/* Redraws the 10×4 character key grid of `UiNameEntry`: clears the `keyText`
   printer and draws the 40 characters of the current page (start index `page`, or 0xa0 when
   `symbolPage` is set) from `g_nameEntryKeyTable`, each twice through the printer's print method
   (outline `g_colorBlack` at +0/+3, then `g_colorWhite` at −1/+2) at x = 162 + 28·col,
   y = base + 26·row, base 72 (71 on the symbol page, 73 on pages 0x50 and 0x78), adjusted by
   `UiNameEntryGetGlyphOffset`. */

typedef void (*UiTextPrintFn)(float x, float y, float z, void *printer, u8 *text, s32 a, s32 b,
                              s32 c);

void UiNameEntryRedrawKeyGrid(UiNameEntry *self)
{
  UiTextPrinter *printer;
  const VtblEntry *entry;
  int start;
  int row;
  int rowBase;
  int col;
  int ch;
  float rowY;
  float baseY;
  float y;
  float x;
  float dx;
  float dy;
  u8 text[4];

  start = self->page;
  if (self->symbolPage != 0) {
    start = 0xa0;
  }
  printer = (UiTextPrinter *)self->keyText;
  GfxSpriteLayerClear(&printer->layer);
  printer->glyphs = NULL;
  row = 0;
  rowBase = 0;
  do {
    rowY = (float)row * 26.0f;
    col = 0;
    ch = start + rowBase;
    do {
      text[0] = 0;
      text[1] = 0;
      text[2] = 0;
      text[3] = 0;
      UiTextEncodeUtf8(text, g_nameEntryKeyTable[ch]);
      if (self->symbolPage != 0) {
        baseY = 72.0f - 1.0f;
      } else if (self->page == 0x50 || self->page == 0x78) {
        baseY = 72.0f + 1.0f;
      } else {
        baseY = 72.0f;
      }
      dx = 0.0f;
      dy = 0.0f;
      UiNameEntryGetGlyphOffset(ch, &dx, &dy);

      /* outline colour black (the print method reads no VFPU register) */
      printer = (UiTextPrinter *)self->keyText;
      printer->outlineColor[0] = g_colorBlack.x;
      printer->outlineColor[1] = g_colorBlack.y;
      printer->outlineColor[2] = g_colorBlack.z;
      printer->outlineColor[3] = g_colorBlack.w;
      printer = (UiTextPrinter *)self->keyText;
      entry = &printer->layer.vtbl[2];
      x = (float)col * 28.0f + 148.0f + 14.0f;
      y = baseY + rowY;
      ((UiTextPrintFn)entry->fn)(x + dx, y + 3.0f + dy, 0.0f, (u8 *)printer + entry->delta, text, 1,
                                 0, 0);

      /* outline colour white */
      printer = (UiTextPrinter *)self->keyText;
      printer->outlineColor[0] = g_colorWhite.x;
      printer->outlineColor[1] = g_colorWhite.y;
      printer->outlineColor[2] = g_colorWhite.z;
      printer->outlineColor[3] = g_colorWhite.w;
      printer = (UiTextPrinter *)self->keyText;
      entry = &printer->layer.vtbl[2];
      ((UiTextPrintFn)entry->fn)((x - 1.0f) + dx, y + 2.0f + dy, 0.0f, (u8 *)printer + entry->delta,
                                 text, 1, 0, 0);
      col++;
      ch++;
    } while (col < 10);
    row++;
    rowBase += 10;
  } while (row < 4);
}
