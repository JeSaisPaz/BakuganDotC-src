// bdc 0x0896d0e4 UiCardEquipPrintCardName
#include "bdc.h"

/* Prints the name of card `cardId` (string table `"DWCardName_eu.bin"` from the pack chain,
   `CorePackChainFind`, `UiMesTableRelocate`) into `nameText` and the card-name printer of
   `UiCardEquip`: clears the printer (`GfxSpriteLayerClear`, glyph list reset)
   and prints through its print method (vtable `+0x74` slot 2, depth 0, flags 1/0/0) 18 px above
   the selected Bakugan's text anchor sprite `groups[19].first + selBakugan`; stores the measured
   width/height/lines (`UiTextMeasure`, no width limit) in `nameWidth`/`nameHeight`/`nameLines`,
   then the printer's glyph list in `nameGlyphs` and its glyph count as a float in
   `nameGlyphCount`. */

void UiCardEquipPrintCardName(UiCardEquip *self, u8 cardId)
{
  u32 *table;
  char *dst;
  UiTextPrinter *printer;
  const VtblEntry *vt;
  GfxSprite *sprite;

  table = CorePackChainFind(g_ioLzsPackages, "DWCardName_eu.bin");
  UiMesTableRelocate(table);
  dst = self->nameText;
  strcpy(dst, (const char *)PspPtr(table[cardId]));
  printer = self->namePrinter;
  GfxSpriteLayerClear(&printer->layer);
  printer->glyphs = NULL;
  printer = self->namePrinter;
  sprite = ((GfxSprite **)self->base.data)[self->groups[19][0] + self->selBakugan];
  vt = &printer->layer.vtbl[2];
  ((void (*)(float, float, float, void *, char *, u32, u32, u32))vt->fn)(
      sprite->posX, sprite->posY - 18.0f, 0.0f, (char *)printer + vt->delta, dst, 1, 0, 0);
  UiTextMeasure(0.0f, self->namePrinter, dst, &self->nameWidth, &self->nameHeight,
                &self->nameLines);
  self->nameGlyphs = self->namePrinter->glyphs;
  self->nameGlyphCount = (float)self->namePrinter->glyphCount;
}
