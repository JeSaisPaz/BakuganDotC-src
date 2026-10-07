// bdc 0x0884c390 BtlIsScoreMode
#include "bdc.h"

/* Returns 1 when the battle main task (id 100, `BtlCameraTaskExists`) is alive, the battle rule
   mode selector (script global variable entry 8 of `g_scriptGlobalVars`) is 2 and the score mode
   (profile word 7) equals `mode`; otherwise 0. `ActorCrystalCtor` and its helpers call it with 1
   to choose the crystal variant of the ranked score battle. */

u32 BtlIsScoreMode(u32 mode)
{
  u32 result;

  result = 0;
  if (BtlCameraTaskExists() != 0 && g_scriptGlobalVars[8] == 2) {
    if (SaveProfileGetWord(SaveGetProfile(), 7) == mode) {
      result = 1;
    }
  }
  return result;
}
