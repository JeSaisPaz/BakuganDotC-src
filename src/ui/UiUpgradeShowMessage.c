// bdc 0x08914848 UiUpgradeShowMessage
#include "bdc.h"

/* Shows line `index` of the `DMUpgrade` message table (empty string for 0) in the message text
   printer `textPrinter` of the Bakugan upgrade screen (`UiUpgradeCtor`, task 490): copies it
   into `message`, clears the printer, prints the text at the position of screen sprite 43
   (y - 6) through printer vtable slot 2 and then shifts every glyph up by half of the largest
   glyph y offset from the first glyph, centring the text vertically. */

void UiUpgradeShowMessage(UiUpgrade *self, u32 index)
{
  u32 *table;
  char *dst;
  UiTextPrinter *printer;
  const VtblEntry *entry;
  GfxSprite *anchor;
  GfxSprite *first;
  GfxSprite *glyph;
  int maxOffset;
  int offset;
  int half;
  int i;

  dst = self->message;
  if ((index & 0xff) == 0) {
    strcpy(dst, "");
  }
  else {
    table = SaveFindLocalizedBin("DMUpgrade");
    UiMesTableRelocate(table);
    strcpy(dst, (const char *)PspPtr(table[(index & 0xff) - 1]));
  }
  printer = self->textPrinter;
  GfxSpriteLayerClear(&printer->layer);
  printer->glyphs = NULL;
  printer = self->textPrinter;
  anchor = ((GfxSprite **)self->base.data)[43];
  entry = &printer->layer.vtbl[2];
  ((void (*)(float, float, float, void *, char *, s32, s32, s32))entry->fn)(
      anchor->posX, anchor->posY - 6.0f, 0.0f, (u8 *)printer + entry->delta, dst, 1, 0, 0);

  printer = self->textPrinter;
  first = printer->glyphs;
  glyph = first;
  maxOffset = 0;
  for (i = 0; i < printer->glyphCount; i++) {
    offset = (int)(glyph->posY - first->posY);
    if (maxOffset < offset) {
      maxOffset = offset;
    }
    glyph = glyph->next;
  }
  half = maxOffset / 2;
  glyph = first;
  for (i = 0; i < self->textPrinter->glyphCount; i++) {
    glyph->posY = glyph->posY - (float)half;
    glyph = glyph->next;
  }
}
