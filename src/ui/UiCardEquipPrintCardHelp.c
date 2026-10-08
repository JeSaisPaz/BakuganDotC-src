// bdc 0x0896d3b0 UiCardEquipPrintCardHelp
#include "bdc.h"

/* Prints the help text of card `cardId` into the help printer of `UiCardEquip`:
   copies entry `cardId` of the `"DWCardHelp_eu.bin"` message table (found in the loaded packs
   `g_ioLzsPackages`) to `helpText`, clears `helpPrinter` and prints it (printer vtable slot 2,
   Print) at the anchor sprite `groups[0x13][0] + selBakugan` (posY, or posY - 5 with 3+ Bakugan),
   measures it with `UiTextMeasure` into `helpWidth/helpHeight/helpLines`, stores the glyph
   list/count in `helpGlyphs`/`helpGlyphCount` and shifts every glyph up by half of the largest
   glyph Y offset from the first glyph (vertical centring of multi-line text). */

void UiCardEquipPrintCardHelp(UiCardEquip *self, u8 cardId)
{
  u32 *table;
  char *dst;
  UiTextPrinter *printer;
  const VtblEntry *entry;
  GfxSprite *anchor;
  GfxSprite *first;
  GfxSprite *glyph;
  float count;
  int maxOffset;
  int offset;
  int i;

  table = CorePackChainFind(g_ioLzsPackages, "DWCardHelp_eu.bin");
  UiMesTableRelocate(table);
  dst = self->helpText;
  strcpy(dst, (const char *)PspPtr(table[cardId]));
  printer = self->helpPrinter;
  GfxSpriteLayerClear(&printer->layer);
  printer->glyphs = NULL;

  printer = self->helpPrinter;
  entry = &printer->layer.vtbl[2];
  anchor = ((GfxSprite **)self->base.data)[self->groups[0x13][0] + self->selBakugan];
  if (self->bakuganCount < 3) {
    ((void (*)(float, float, float, void *, char *, s32, s32, s32))entry->fn)(
        anchor->posX, anchor->posY, 0.0f, (u8 *)printer + entry->delta, dst, 1, 0, 0);
  }
  else {
    ((void (*)(float, float, float, void *, char *, s32, s32, s32))entry->fn)(
        anchor->posX, anchor->posY - 5.0f, 0.0f, (u8 *)printer + entry->delta, dst, 1, 0, 0);
  }
  UiTextMeasure(0.0f, self->helpPrinter, dst, &self->helpWidth, &self->helpHeight,
                &self->helpLines);

  printer = self->helpPrinter;
  self->helpGlyphs = printer->glyphs;
  first = self->helpGlyphs;
  maxOffset = 0;
  count = (float)printer->glyphCount;
  i = 0;
  self->helpGlyphCount = count;
  if (0.0f < count) {
    glyph = first;
    do {
      offset = (int)(glyph->posY - first->posY);
      if (maxOffset < offset) {
        maxOffset = offset;
      }
      i++;
      glyph = glyph->next;
    } while ((float)i < count);
  }
  i = 0;
  if (0.0f < count) {
    do {
      i++;
      first->posY = first->posY - (float)(maxOffset / 2);
      first = first->next;
    } while ((float)i < self->helpGlyphCount);
  }
}
