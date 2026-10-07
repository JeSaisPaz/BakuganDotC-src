// bdc 0x088058a4 UiNameEntryRedrawName
#include "bdc.h"

/* Redraws the typed name of `UiNameEntry`: clears the name text printer `+0x94`
   (and its last-print glyph pointer) and prints each of the 12 character slots (`+0xc8`, -1 = empty)
   from the key-label table `g_nameEntryKeyTable` (converted to font codes by `UiTextEncodeUtf8`)
   at x = 170.5 + 21·i (+ half the glyph x offset from `UiNameEntryGetGlyphOffset`),
   y = 42 + glyph y offset, through the printer's virtual print method (vtable `+0x74`, slot 2). */

void UiNameEntryRedrawName(UiNameEntry *self)

{
  UiTextPrinter *printer;
  const VtblEntry *vt;
  int i;
  u8 text[4];
  float dx;
  float dy;

  printer = (UiTextPrinter *)self->nameText;
  GfxSpriteLayerClear(&printer->layer);
  printer->glyphs = (GfxSprite *)0x0;
  for (i = 0; i < 12; i++) {
    if (self->name[i] != -1) {
      text[0] = 0;
      text[1] = 0;
      text[2] = 0;
      text[3] = 0;
      UiTextEncodeUtf8(text, g_nameEntryKeyTable[self->name[i]]);
      dx = 0.0f;
      dy = 0.0f;
      UiNameEntryGetGlyphOffset(self->name[i], &dx, &dy);
      dx = dx * 0.5f;
      printer = (UiTextPrinter *)self->nameText;
      vt = printer->layer.vtbl + 2;
      ((void (*)(float, float, float, void *, u8 *, u8, u8, u8))vt->fn)(
          (float)i * 21.0f + 160.0f + 10.5f + dx, dy + 42.0f, 0.0f, (char *)printer + vt->delta,
          text, 1, 0, 0);
    }
  }
}
