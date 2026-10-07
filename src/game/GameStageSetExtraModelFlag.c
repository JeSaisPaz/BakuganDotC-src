// bdc 0x088d4738 GameStageSetExtraModelFlag
#include "bdc.h"

/* Stores `value` in `g_gameStageExtraModelFlag` and in byte `+0xbc` of the up to three extra stage models at
   `g_gameStageExtraModels` (world-map model and sub-models). */

void GameStageSetExtraModelFlag(u8 value)
{
  s32 i;

  g_gameStageExtraModelFlag = value;
  for (i = 0; i < 3; i++) {
    if (g_gameStageExtraModels[i] != 0) {
      g_gameStageExtraModels[i][0xbc] = value;
    }
  }
}
