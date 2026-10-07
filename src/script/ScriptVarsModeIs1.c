// bdc 0x0880d338 ScriptVarsModeIs1
#include "bdc.h"

/* True when the game-mode word of the script global variables (`*0x08ac58c4 + 0x20`) is 1. In that
   mode the partner Bakugan is replaced by the fixed id 0x15 (`SaveGetTeamBakuganOrMode1`) / face
   0x2f (`SaveGetTeamBakuganFace`), and `BtlDemoFinish` restores the field actor. */

bool ScriptVarsModeIs1(void)

{
  return g_scriptGlobalVars[8] == 1;
}

