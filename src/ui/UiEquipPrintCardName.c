// bdc 0x08961c6c UiEquipPrintCardName
#include "bdc.h"

/* Prints the name of card `cardId` in the help pop-up of the UiEquip Bakugan/gear loadout screen
   (task 302, `UiEquipCtor`): looks it up in the localized `"DWCardName"` table
   (`SaveFindLocalizedBin`, `UiMesTableRelocate`), copies it to `nameText` (`+0x5030`), clears
   the name printer `+0x5028` (`GfxSpriteLayerClear`, glyph list reset) and prints it through the
   printer's print method (vtable `+0x74` slot 2, `UiTextPrinterPrint`, depth 0, flags 1/0/0)
   40 px above the edited player's sprite `spriteIdx[0x13] + editPlayer`; stores the measured
   width/height/lines (`UiTextMeasure`, no width limit) at `+0x50f0/+0x50f8/+0x5100`, then the
   printer's glyph list at `+0x50e8` and its glyph count as a float at `+0x50e0`. */

void UiEquipPrintCardName(UiEquip *self, u8 cardId)
{
  u32 *table;
  char *dst;
  UiTextPrinter *printer;
  const VtblEntry *vt;
  GfxSprite *sprite;

  table = SaveFindLocalizedBin("DWCardName");
  UiMesTableRelocate(table);
  dst = self->nameText;
  strcpy(dst, (const char *)PspPtr(table[cardId]));
  printer = self->namePrinter;
  GfxSpriteLayerClear(&printer->layer);
  printer->glyphs = NULL;
  printer = self->namePrinter;
  sprite = ((GfxSprite **)self->base.data)[self->spriteIdx[0x13] + self->editPlayer];
  vt = &printer->layer.vtbl[2];
  ((void (*)(float, float, float, void *, char *, u32, u32, u32))vt->fn)(
      sprite->posX, sprite->posY - 40.0f, 0.0f, (char *)printer + vt->delta, dst, 1, 0, 0);
  UiTextMeasure(0.0f, self->namePrinter, dst, &self->nameWidth, &self->nameHeight,
                &self->nameLines);
  self->nameGlyphs = self->namePrinter->glyphs;
  self->nameGlyphCount = (float)self->namePrinter->glyphCount;
}
