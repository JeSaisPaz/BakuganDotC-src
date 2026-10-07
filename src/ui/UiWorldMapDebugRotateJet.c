// bdc 0x0899c130 UiWorldMapDebugRotateJet
#include "bdc.h"

/* Debug control of `UiWorldMap` (main-phase step 0x10): on the pad's repeat bits
   rotates `jetPitch` (up +, down -), `jetYaw` (left +, right -) and `jetRoll` (L +, R -) by
   0.05 rad, then wraps all three with `UiWrapAngle`. */

void UiWorldMapDebugRotateJet(UiScreen *screen)
{
  UiWorldMap *map = (UiWorldMap *)screen;
  PadState *pad = screen->pad;

  if (pad->repeat & 0x10) {
    map->jetPitch = map->jetPitch + 0.05f;
  }
  if (pad->repeat & 0x40) {
    map->jetPitch = map->jetPitch - 0.05f;
  }
  if (pad->repeat & 0x80) {
    map->jetYaw = map->jetYaw + 0.05f;
  }
  if (pad->repeat & 0x20) {
    map->jetYaw = map->jetYaw - 0.05f;
  }
  if (pad->repeat & 0x100) {
    map->jetRoll = map->jetRoll + 0.05f;
  }
  if (pad->repeat & 0x200) {
    map->jetRoll = map->jetRoll - 0.05f;
  }
  map->jetPitch = UiWrapAngle(map->jetPitch);
  map->jetYaw = UiWrapAngle(map->jetYaw);
  map->jetRoll = UiWrapAngle(map->jetRoll);
}
