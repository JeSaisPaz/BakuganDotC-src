// bdc 0x08985548 UiCollectionCardSetHelpText
#include "bdc.h"

/* Fills the description box of `UiCollectionCard` with the `"DWCardHelp"`
   entry of the selected card (`SaveFindLocalizedBin`) at the anchor sprite 0x40, measures it
   (`UiTextMeasure`) and centres the lines. */

void UiCollectionCardSetHelpText(UiCollectionCard *self)
{
  u8 card;
  u32 *table;
  char *dst;
  UiTextPrinter *printer;
  GfxSprite *anchor;
  const VtblEntry *entry;
  GfxSprite *first;
  GfxSprite *glyph;
  float count;
  int maxOffset;
  int offset;
  int i;

  card = self->slots[self->cursor + self->page * 4];
  table = SaveFindLocalizedBin("DWCardHelp");
  UiMesTableRelocate(table);
  dst = self->helpText;
  strcpy(dst, ((char **)table)[card]);
  printer = self->helpPrinter;
  GfxSpriteLayerClear(&printer->layer);
  printer->glyphs = NULL;

  printer = self->helpPrinter;
  anchor = ((GfxSprite **)self->base.data)[64];
  entry = &printer->layer.vtbl[2];
  ((void (*)(float, float, float, void *, char *, s32, s32, s32))entry->fn)(
      anchor->posX, anchor->posY, 0.0f, (u8 *)printer + entry->delta, dst, 1, 0, 0);
  UiTextMeasure(0.0f, self->helpPrinter, dst, &self->helpWidth, &self->helpTextH, &self->helpLines);

  printer = self->helpPrinter;
  self->helpGlyphs = printer->glyphs;
  count = (float)printer->glyphCount;
  first = self->helpGlyphs;
  maxOffset = 0;
  i = 0;
  self->helpHeight = count;
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
    glyph = first;
    do {
      i++;
      glyph->posY = glyph->posY - (float)(maxOffset / 2);
      glyph = glyph->next;
    } while ((float)i < self->helpHeight);
  }
}
