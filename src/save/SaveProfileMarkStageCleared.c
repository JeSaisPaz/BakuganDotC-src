// bdc 0x089b2058 SaveProfileMarkStageCleared
#include "bdc.h"

/* Records a won story stage (`stage` = script global 1, `g_scriptGlobalVars[1]`) in the profile's
   progress bitmaps: sets a per-stage-group bit in `storyAreaMask` (stages 4..7 → 2, 8..11 → 4, 14 → 8,
   18 → 0x10, 20..23 and 37 → 0x20, 24..27 → 0x40), then for stages other than 20..36 and 38/39
   sets bit `stage >> 2` of `areaCleared` and bit `stage` of `stageCleared`; stage 37 instead sets
   area bit 5 (`areaCleared[0] |= 0x20`) and stage bit 20 (`stageCleared[2] |= 0x10`). */

void SaveProfileMarkStageCleared(void)
{
  s32 stage;
  s32 area;
  s32 bit;
  u8 hi;
  u8 lo;
  SaveProfileData *data;

  stage = g_scriptGlobalVars[1];
  switch (stage) {
  case 4: case 5: case 6: case 7:
    SaveGetProfile()->data->storyAreaMask |= 2;
    stage = g_scriptGlobalVars[1];
    break;
  case 8: case 9: case 10: case 11:
    SaveGetProfile()->data->storyAreaMask |= 4;
    stage = g_scriptGlobalVars[1];
    break;
  case 14:
    SaveGetProfile()->data->storyAreaMask |= 8;
    stage = g_scriptGlobalVars[1];
    break;
  case 18:
    SaveGetProfile()->data->storyAreaMask |= 0x10;
    stage = g_scriptGlobalVars[1];
    break;
  case 20: case 21: case 22: case 23:
    SaveGetProfile()->data->storyAreaMask |= 0x20;
    stage = g_scriptGlobalVars[1];
    break;
  case 24: case 25: case 26: case 27:
    SaveGetProfile()->data->storyAreaMask |= 0x40;
    stage = g_scriptGlobalVars[1];
    break;
  case 37:
    SaveGetProfile()->data->storyAreaMask |= 0x20;
    stage = g_scriptGlobalVars[1];
    break;
  default:
    break;
  }

  switch (stage) {
  case 20: case 21: case 22: case 23: case 24: case 25: case 26: case 27:
  case 28: case 29: case 30: case 31: case 32: case 33: case 34: case 35:
  case 36: case 38: case 39:
    return;
  default:
    break;
  }

  if (stage == 37) {
    SaveGetProfile()->data->areaCleared[0] |= 0x20;
    SaveGetProfile()->data->stageCleared[2] |= 0x10;
    return;
  }

  data = SaveGetProfile()->data;
  area = (u8)(g_scriptGlobalVars[1] / 4);
  data->areaCleared[area / 8] |= 1 << (area % 8);

  data = SaveGetProfile()->data;
  stage = g_scriptGlobalVars[1];
  hi = stage / 4;
  lo = stage % 4;
  bit = hi * 4 + lo;
  data->stageCleared[bit / 8] |= 1 << (bit % 8);
}
