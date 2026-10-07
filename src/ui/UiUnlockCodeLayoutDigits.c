// bdc 0x089927bc UiUnlockCodeLayoutDigits
#include "bdc.h"

/* Rebuilds the text of the entered code in `UiUnlockCode`: clears the text layer
   `digitText`, then for each of the `digitCount` digit slots `entered[i]` that is set (not −1) draws
   the character from `g_unlockKeyTable` (UTF-8 encoded) at x = 122.5 + 21·i (+ half the glyph
   offset from `UiUnlockCodeGetGlyphOffset`), y = 42 + glyph dy, through the printer's vtable
   slot 2. */

typedef void (*UiTextPrinterPutFn)(void *self, const u8 *text, int a2, int a3, int t0, float x,
                                   float y, float z);

void UiUnlockCodeLayoutDigits(UiUnlockCode *self)
{
  UiTextPrinter *printer;
  const VtblEntry *entry;
  int i;
  u8 text[4];
  float dx;
  float dy;

  printer = self->digitText;
  GfxSpriteLayerClear(&printer->layer);
  printer->glyphs = NULL;
  for (i = 0; i < self->digitCount; i++) {
    if (self->entered[i] == -1) {
      continue;
    }
    text[0] = 0;
    text[1] = 0;
    text[2] = 0;
    text[3] = 0;
    UiTextEncodeUtf8(text, g_unlockKeyTable[self->entered[i]]);
    dx = 0.0f;
    dy = 0.0f;
    UiUnlockCodeGetGlyphOffset(self->entered[i], &dx, &dy);
    dx = dx * 0.5f;
    printer = self->digitText;
    entry = &printer->layer.vtbl[2];
    ((UiTextPrinterPutFn)entry->fn)((u8 *)printer + entry->delta, text, 1, 0, 0,
                                    (float)i * 21.0f + 112.0f + 10.5f + dx, dy + 42.0f, 0.0f);
  }
}
