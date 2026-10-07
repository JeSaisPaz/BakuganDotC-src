// bdc 0x0899ed90 UiWorldMapApplyNetCursor
#include "bdc.h"

/* Applies the cursor part of a network state packet from player `slot` (0..3) to
   `UiWorldMap`: stores words 1 and 2 in the player record (`+0x2332`, `+0x2334`),
   in a network session copies word 2 into the cancel flag `+0x109f`, and moves the cursor through
   `UiWorldMapSetPlayerSelection` with word 0. */

void UiWorldMapApplyNetCursor(UiScreen *screen, int slot, s16 *data)

{
  UiWorldMap *map = (UiWorldMap *)screen;
  s16 sel;

  if ((-1 < slot) && (slot < 4)) {
    map->player[slot].cursorA = data[1];
    map->player[slot].cursorB = data[2];
    if (map->netSession == 0) {
      sel = *data;
    }
    else {
      map->cancelFlag = (u8)data[2];
      sel = *data;
    }
    UiWorldMapSetPlayerSelection(screen, slot, (int)sel);
  }
}
