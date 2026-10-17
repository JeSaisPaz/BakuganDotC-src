// bdc 0x089982cc UiWorldMapAngleDelta
#include "bdc.h"

/* Shortest turn from angle `from` to `to` (radians, period 6.28) for the
   `UiWorldMap` globe: writes the `UiWorldMapTurnDelta` `{u8 dir, float dist}` to
   `out` (`dir` 1 = increase, 0 = decrease). */

void UiWorldMapAngleDelta(float from, float to, UiWorldMapTurnDelta *out)

{
  UiWorldMapTurnDelta r;
  float a;
  float b;

  memset(&r, 0, 8);
  if (from < to) {
    b = (from + 6.28f) - to;
    a = to - from;
    if (a <= b) {
      r.dir = 1;
      r.dist = a;
    }
    else {
      r.dir = 0;
      r.dist = b;
    }
  }
  else {
    a = from - to;
    b = (to + 6.28f) - from;
    if (a <= b) {
      r.dir = 0;
      r.dist = a;
    }
    else {
      r.dir = 1;
      r.dist = b;
    }
  }
  *(u32 *)out = *(u32 *)&r;
  out->dist = r.dist;
}
