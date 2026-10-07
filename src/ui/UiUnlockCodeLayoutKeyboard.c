// bdc 0x089929dc UiUnlockCodeLayoutKeyboard
#include "bdc.h"

/* Rebuilds the on-screen keyboard text of `UiUnlockCode`: clears the `keyText`
   printer and draws the 4×10 characters of the current character page (start index `pageOffset`,
   or 0xa0 when `page` is set) from `g_unlockKeyTable`, each twice through the printer's print
   method (outline `g_colorBlack` at +0/+3, then `g_colorWhite` at −1/+2) at
   x = 114 + 28·col, y = base + 26·row, base 72 (71 on the extra page, 73 on pages 0x50 and 0x78),
   adjusted by `UiUnlockCodeGetGlyphOffset`. */

typedef void (*UiTextPrintFn)(float x, float y, float z, void *printer, u8 *text, s32 a, s32 b,
                              s32 c);

void UiUnlockCodeLayoutKeyboard(UiUnlockCode *self)
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

  start = self->pageOffset;
  if (self->page != 0) {
    start = 0xa0;
  }
  printer = self->keyText;
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
      UiTextEncodeUtf8(text, g_unlockKeyTable[ch]);
      if (self->page != 0) {
        baseY = 72.0f - 1.0f;
      } else if (self->pageOffset == 0x50 || self->pageOffset == 0x78) {
        baseY = 72.0f + 1.0f;
      } else {
        baseY = 72.0f;
      }
      dx = 0.0f;
      dy = 0.0f;
      UiUnlockCodeGetGlyphOffset(ch, &dx, &dy);

      printer = self->keyText;
      printer->outlineColor[0] = g_colorBlack.x;
      printer->outlineColor[1] = g_colorBlack.y;
      printer->outlineColor[2] = g_colorBlack.z;
      printer->outlineColor[3] = g_colorBlack.w;
      printer = self->keyText;
      entry = &printer->layer.vtbl[2];
      x = (float)col * 28.0f + 100.0f + 14.0f;
      y = baseY + rowY;
      ((UiTextPrintFn)entry->fn)(x + dx, y + 3.0f + dy, 0.0f, (u8 *)printer + entry->delta, text, 1,
                                 0, 0);

      printer = self->keyText;
      printer->outlineColor[0] = g_colorWhite.x;
      printer->outlineColor[1] = g_colorWhite.y;
      printer->outlineColor[2] = g_colorWhite.z;
      printer->outlineColor[3] = g_colorWhite.w;
      printer = self->keyText;
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
