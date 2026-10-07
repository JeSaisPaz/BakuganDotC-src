// bdc 0x0899a600 UiWorldMapGetJetAngle
#include "bdc.h"

/* Returns the jet orientation angle `axis` (0..2) for flight direction `dir` (0..3) of
   `UiWorldMap` from the 4×3 float table `g_worldMapJetAngles` (e.g. dir 0 = {5.53, 2.63,
   5.68}, dir 2 = {4.68, 3.14, 0}). */

float UiWorldMapGetJetAngle(UiScreen *screen, u8 dir, u8 axis)
{
  float angles[12];

  memcpy(angles, g_worldMapJetAngles, 0x30);
  return angles[(u32)axis + (u32)dir * 3];
}
