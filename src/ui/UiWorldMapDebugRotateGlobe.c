// bdc 0x0899c014 UiWorldMapDebugRotateGlobe
#include "bdc.h"

/* Debug control of `UiWorldMap` (main-phase step 0xf): on the pad's repeat bits
   rotates `globePitch` (up +, down -), `globeYaw` (left +, right -) and `globeRoll` (L +, R -) by
   0.05 rad, then wraps all three with `UiWrapAngle`. */

void UiWorldMapDebugRotateGlobe(UiScreen *screen)
{
  UiWorldMap *map = (UiWorldMap *)screen;
  PadState *pad = screen->pad;

  if (pad->repeat & 0x10) {
    map->globePitch = map->globePitch + 0.05f;
  }
  if (pad->repeat & 0x40) {
    map->globePitch = map->globePitch - 0.05f;
  }
  if (pad->repeat & 0x80) {
    map->globeYaw = map->globeYaw + 0.05f;
  }
  if (pad->repeat & 0x20) {
    map->globeYaw = map->globeYaw - 0.05f;
  }
  if (pad->repeat & 0x100) {
    map->globeRoll = map->globeRoll + 0.05f;
  }
  if (pad->repeat & 0x200) {
    map->globeRoll = map->globeRoll - 0.05f;
  }
  map->globePitch = UiWrapAngle(map->globePitch);
  map->globeYaw = UiWrapAngle(map->globeYaw);
  map->globeRoll = UiWrapAngle(map->globeRoll);
}
