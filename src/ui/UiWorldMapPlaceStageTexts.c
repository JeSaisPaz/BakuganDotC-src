// bdc 0x0899c618 UiWorldMapPlaceStageTexts
#include "bdc.h"

/* Keeps the text slots of `UiWorldMap` on the stage-list plates: for each of the
   selected area's cleared stages (`clearedStage[areaGroup[areaId]]`, at most 3) moves the slot's
   glyph sprites to plate sprite `0x32 + i`'s Y − 19 and copies the plate's alpha into the slot
   alpha. */

void UiWorldMapPlaceStageTexts(UiScreen *screen)

{
  UiWorldMap *map = (UiWorldMap *)screen;
  GfxSprite **sprites = (GfxSprite **)screen->data;
  int i;

  for (i = 0; i < map->clearedStage[map->areaGroup[map->areaId]]; ) {
    GfxSprite *glyph = map->textSlot[i].glyphs;
    GfxSprite *plate = sprites[0x32 + i];
    int n = 0;

    if (0.0f < map->textSlot[i].glyphCount) {
      do {
        n++;
        glyph->posY = plate->posY - 19.0f;
        plate = sprites[0x32 + i];
        glyph = glyph->next;
      } while ((float)n < map->textSlot[i].glyphCount);
    }
    i++;
    map->textSlot[i - 1].alpha = plate->alpha;
    if (i >= 3) {
      break;
    }
  }
  return;
}
