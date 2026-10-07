// bdc 0x0889da98 GameStageIs20To27Or36To39
#include "bdc.h"

/* Returns 1 if the current stage number (script global 1, `*(s32 *)(g_scriptGlobalVars + 4)`) is
   0x14..0x1b or 0x24..0x27, else 0. `BtlStageLoadMap` skips the sun/moon sprites and the extra
   camera task 0x1e2 for these stages. */

s32 GameStageIs20To27Or36To39(void)
{
  switch (g_scriptGlobalVars[1]) {
  case 0x14:
  case 0x15:
  case 0x16:
  case 0x17:
  case 0x18:
  case 0x19:
  case 0x1a:
  case 0x1b:
  case 0x24:
  case 0x25:
  case 0x26:
  case 0x27:
    return 1;
  default:
    return 0;
  }
}
