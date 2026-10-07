// bdc 0x0899990c UiWorldMapIdleSpin
#include "bdc.h"

/* Idle spin of the `UiWorldMap` globe: turns the Y rotation `+0x21d4` by 0.002 rad
   per frame in the direction `+0x2269` (wrapped by `UiWrapAngle`). */

void UiWorldMapIdleSpin(UiScreen *screen)
{
  UiWorldMap *map = (UiWorldMap *)screen;

  if (map->spinForward == 0) {
    map->globeYaw = UiWrapAngle(map->globeYaw - 0.002f);
  } else {
    map->globeYaw = UiWrapAngle(map->globeYaw + 0.002f);
  }
}
