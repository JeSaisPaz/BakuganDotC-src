// bdc 0x0889da2c GameStageIs0Or13
#include "bdc.h"

/* Returns 1 if the current stage number (script global variable 1: `*(s32 *)(g_scriptGlobalVars +
   4)`, the global variable table pointer) is 0 or 13, else 0. Used by 16 callers that special-case
   those stages (actor tinting in `ActorSpawn`, `ActorCrystalSetStyle`, `BtlCreateBakugan`,
   ...). */

s32 GameStageIs0Or13(void)
{
  s32 stage = g_scriptGlobalVars[1];

  if (stage == 0 || stage == 13) {
    return 1;
  }
  return 0;
}
