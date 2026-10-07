// bdc 0x0899a590 UiWorldMapJetWobbleStep
#include "bdc.h"

/* Advances the jet wobble timer `jetWobbleTimer` of `UiWorldMap` by 1/120 and sets
   the wobble angle `jetWobble` to `0.1 * (1 - cos(pi * t))` (0..0.2 rad). */

void UiWorldMapJetWobbleStep(UiScreen *screen)
{
  UiWorldMap *map = (UiWorldMap *)screen;
  float t;

  t = map->jetWobbleTimer + 0.008333334f;
  map->jetWobbleTimer = t;
  map->jetWobble = (1.0f - __builtin_cosf(t * 3.1415927f)) * 0.5f * 0.2f;
}
