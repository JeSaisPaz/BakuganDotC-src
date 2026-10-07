// bdc 0x088d4050 GameGetStageLight
#include "bdc.h"

/* Returns the address of row `index` of the stage lighting table at `g_gameStageLights` (row stride
   `0x3c`); `-1` means the current row `g_gameStageIndex`. A row holds two colours read by
   `ActorApplyStageLight`: a vec3 at `+0x0c` and another at `+0x30`. */

void *GameGetStageLight(s32 index)

{
  if (index == -1) {
    index = g_gameStageIndex;
  }
  return (u8 *)g_gameStageLights + index * 0x3c;
}

