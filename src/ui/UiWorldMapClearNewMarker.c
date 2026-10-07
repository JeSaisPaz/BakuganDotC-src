// bdc 0x0899ec14 UiWorldMapClearNewMarker
#include "bdc.h"

/* Outside rank mode, hides the "new" marker (sprite `0x10 + area`) of the selected area of
   `UiWorldMap`. */

void UiWorldMapClearNewMarker(UiScreen *screen)
{
  UiWorldMap *map = (UiWorldMap *)screen;

  if (!UiWorldMapIsRankMode(screen)) {
    struct { u8 head[0x40]; struct { u8 head[0xd0]; u32 flags; } *sprites[]; } *data = screen->data;

    data->sprites[map->areaId]->flags &= ~1u;
  }
}
