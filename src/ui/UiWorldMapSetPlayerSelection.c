// bdc 0x0899ecf4 UiWorldMapSetPlayerSelection
#include "bdc.h"

/* Stores the selection `value` of network player `slot` (0..3) of `UiWorldMap` in
   the record `+0x2330 + slot * 0x10`; for slot 0 in a network session (`+0x2370` ≠ 0) also moves
   the area cursor `+0x109c` there with the cursor sound and redraw
   (`UiWorldMapRefreshAreaCursor`). */

void UiWorldMapSetPlayerSelection(UiScreen *screen, int slot, int value)

{
  UiWorldMap *map = (UiWorldMap *)screen;

  if (slot >= 0 && slot < 4 && map->player[slot].selection != value) {
    map->player[slot].selection = (s16)value;
    if (slot == 0 && map->netSession != 0 && map->areaId != value) {
      map->areaId = (s8)value;
      if (SndHasManager()) {
        SndManagerPlay(SndGetManager(), 1, 0, 0);
      }
      UiWorldMapRefreshAreaCursor(screen);
    }
  }
}
