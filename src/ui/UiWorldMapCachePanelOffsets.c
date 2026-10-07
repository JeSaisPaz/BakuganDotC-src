// bdc 0x08997a3c UiWorldMapCachePanelOffsets
#include "bdc.h"

/* Caches the layout of the area panel of `UiWorldMap`: the position of sprite 0x32
   (data slot 0x32, `+200`) in `panelPos`, the Y gap from it down to sprite 0x33 in `panelGapY`, and
   the offsets of nine related sprites (0x36, 0x3d, 0x3a, 0x40, 0x43, 0x46, 0x37, 0x4b, 0x58)
   relative to it in `panelOffset`, so the panel can be moved as a group. */

void UiWorldMapCachePanelOffsets(UiScreen *screen)

{
  static const u8 slot[9] = {0x36, 0x3d, 0x3a, 0x40, 0x43, 0x46, 0x37, 0x4b, 0x58};
  UiWorldMap *map = (UiWorldMap *)screen;
  GfxSprite **spr = (GfxSprite **)screen->data;
  int i;

  map->panelPos[0] = spr[0x32]->posX;
  map->panelPos[1] = spr[0x32]->posY;
  map->panelGapY = spr[0x33]->posY - spr[0x32]->posY;
  for (i = 0; i < 9; i++) {
    map->panelOffset[i][0] = spr[0x32]->posX - spr[slot[i]]->posX;
    map->panelOffset[i][1] = spr[0x32]->posY - spr[slot[i]]->posY;
  }
}
