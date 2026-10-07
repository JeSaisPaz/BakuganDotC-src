// bdc 0x0888f498 GameStageIs36Or38
#include "bdc.h"

/* Returns 1 when the current stage (script global variable 1, `*0x08ac58c4 + 4`) is 0x24 or 0x26,
   else 0. Used by the AI command filter `BtlAiIsCommandAllowed` and `BtlAiExecDetourMove`. */

s32 GameStageIs36Or38(void)
{
  if (g_scriptGlobalVars[1] == 0x24 || g_scriptGlobalVars[1] == 0x26) {
    return 1;
  }
  return 0;
}
