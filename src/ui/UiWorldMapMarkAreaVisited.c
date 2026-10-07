// bdc 0x0899ac50 UiWorldMapMarkAreaVisited
#include "bdc.h"

/* Outside rank mode, sets the bit of the selected area's id (`+0x10a0[+0x109c]`) in the profile
   bitfield at `+0x8d` when leaving `UiWorldMap` for it. */

void UiWorldMapMarkAreaVisited(UiScreen *screen)

{
  UiWorldMap *map = (UiWorldMap *)screen;
  SaveProfile *profile;
  int bit;

  if (!UiWorldMapIsRankMode(screen)) {
    profile = SaveGetProfile();
    bit = map->areaGroup[map->areaId];
    profile->data->areaVisited[bit / 8] |= (u8)(1 << (bit % 8));
  }
}
