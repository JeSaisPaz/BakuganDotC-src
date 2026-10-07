// bdc 0x0889da68 GameStageIs4To7
#include "bdc.h"

/* Returns 1 if the current stage number (script global variable 1: `*(s32 *)(g_scriptGlobalVars +
   4)`) is in 4..7, else 0. Callers: `ActorSpawn`, `ActorCrystalCtor`,
   `BtlBakuganCreateAttachments`, `BtlBakuganInitMotions`, `BtlStageLoadMap`,
   `GameStageBuild`. */

s32 GameStageIs4To7(void)
{
  if (g_scriptGlobalVars[1] >= 4 && g_scriptGlobalVars[1] < 8) {
    return 1;
  }
  return 0;
}
