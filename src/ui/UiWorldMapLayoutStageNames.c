// bdc 0x0899d220 UiWorldMapLayoutStageNames
#include "bdc.h"

/* Writes the stage names of the selected area of `UiWorldMap` into the text slots:
   clears `glyphOffset`, then for each cleared stage (`clearedStage[areaGroup[areaId]]`, at most 3)
   copies entry `areaId * 3 + i` of `"DWWorldName"` (`SaveFindLocalizedBin`) to the slot string,
   clears the slot printer (`GfxSpriteLayerClear`), prints the string at the panel position
   (`panelPos` Y − 19) through the printer's print method, measures it (`UiTextMeasure`) into
   `textW`/`textH`/`textLines`, takes over the printer's glyph list and count, and sets each glyph
   sprite to centred pivot (`GfxSpriteCenterPivot`), flag 0x20 (smooth filtering), scale 0.8 (`UiSpriteSetScaleRotation`),
   Z −40 and Y = plate sprite `0x32 + i` Y − 19, storing the plate-minus-glyph offsets in
   `glyphOffset[i]`. Each slot ends with alpha 0, drawn alpha 1. */

void UiWorldMapLayoutStageNames(UiScreen *screen)
{
  UiWorldMap *map = (UiWorldMap *)screen;
  u32 *table;
  UiTextPrinter *printer;
  const VtblEntry *entry;
  GfxSprite *glyph;
  GfxSprite *plate;
  int area;
  int i;
  int n;

  memset(map->glyphOffset, 0, sizeof(map->glyphOffset));
  table = SaveFindLocalizedBin("DWWorldName");
  UiMesTableRelocate(table);
  for (i = 0; i < 3; i++) {
    area = map->areaId;
    if (!(i < map->clearedStage[map->areaGroup[area]])) {
      return;
    }
    strcpy(map->textSlot[i].text, (const char *)PspPtr(table[area * 3 + i]));
    printer = map->textSlot[i].printer;
    GfxSpriteLayerClear(&printer->layer);
    printer->glyphs = NULL;
    printer = map->textSlot[i].printer;
    entry = &printer->layer.vtbl[2];
    ((void (*)(float, float, float, void *, char *, s32, s32, s32))entry->fn)(
        map->panelPos[0], map->panelPos[1] - 19.0f, 0.0f, (u8 *)printer + entry->delta,
        map->textSlot[i].text, 1, 0, 0);
    UiTextMeasure(0.0f, map->textSlot[i].printer, map->textSlot[i].text, &map->textSlot[i].textW,
                  &map->textSlot[i].textH, &map->textSlot[i].textLines);
    printer = map->textSlot[i].printer;
    map->textSlot[i].glyphs = printer->glyphs;
    glyph = map->textSlot[i].glyphs;
    n = 0;
    map->textSlot[i].glyphCount = (float)printer->glyphCount;
    if (0.0f < map->textSlot[i].glyphCount) {
      do {
        GfxSpriteCenterPivot(glyph);
        glyph->flags |= 0x20;
        UiSpriteSetScaleRotation(glyph, 0.8f, 0.8f, 0.0f);
        glyph->posZ = -40.0f;
        glyph->posY = ((GfxSprite **)screen->data)[0x32 + i]->posY - 19.0f;
        n++;
        plate = ((GfxSprite **)screen->data)[0x32 + i];
        map->glyphOffset[i][n - 1][0] = plate->posX - glyph->posX;
        map->glyphOffset[i][n - 1][1] = plate->posY - glyph->posY;
        glyph = glyph->next;
      } while ((float)n < map->textSlot[i].glyphCount);
    }
    map->textSlot[i].alpha = 0.0f;
    map->textSlot[i].drawnAlpha = 1.0f;
  }
}
