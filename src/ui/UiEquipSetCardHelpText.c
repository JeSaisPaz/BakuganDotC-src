// bdc 0x08961d8c UiEquipSetCardHelpText
#include "bdc.h"

/* Fills the help pop-up of the Bakugan/gear loadout screen before a battle (task 302,
   `UiEquipCtor`) with entry `index` of the `"DWCardHelp"` text table: copies it to `helpText`,
   clears `helpPrinter` and prints it (printer vtable slot 2, Print) at the anchor sprite
   `spriteIdx[0x13] + editPlayer` (posY + 8), measures it with `UiTextMeasure` into
   `helpWidth/helpHeight/helpLines`, stores the glyph list/count in `helpGlyphs`/`helpGlyphCount`
   and shifts every glyph up by half of the largest glyph Y offset from the first glyph
   (vertical centring of multi-line text). */

void UiEquipSetCardHelpText(UiEquip *self, u8 index)
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

  table = SaveFindLocalizedBin("DWCardHelp");
  UiMesTableRelocate(table);
  dst = self->helpText;
  strcpy(dst, ((char **)table)[index]);
  printer = self->helpPrinter;
  GfxSpriteLayerClear(&printer->layer);
  printer->glyphs = NULL;

  printer = self->helpPrinter;
  entry = &printer->layer.vtbl[2];
  anchor = ((GfxSprite **)self->base.data)[self->spriteIdx[0x13] + self->editPlayer];
  /* The 2-player (playerCount < 3) and 4-player branches compile to the identical call. */
  ((void (*)(float, float, float, void *, char *, s32, s32, s32))entry->fn)(
      anchor->posX, anchor->posY - -8.0f, 0.0f, (u8 *)printer + entry->delta, dst, 1, 0, 0);
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
