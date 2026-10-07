// bdc 0x0899ef58 UiWorldMapApplyNetState
#include "bdc.h"

/* Entry point for an 8-short network state packet from player `slot` for
   `UiWorldMap` (also called by `UiOptionNetMainPhase`): passes words 0..3 to
   `UiWorldMapApplyNetCursor` and words 4..7 to `UiWorldMapApplyNetSettings`. */

void UiWorldMapApplyNetState(UiScreen *screen, int slot, s16 *data)

{
  s16 local[8];

  local[0] = data[0];
  local[1] = data[1];
  local[2] = data[2];
  local[3] = data[3];
  local[4] = data[4];
  local[5] = data[5];
  local[6] = data[6];
  local[7] = data[7];
  UiWorldMapApplyNetCursor(screen, slot, &local[0]);
  UiWorldMapApplyNetSettings(screen, slot, (s8 *)&local[4]);
}
