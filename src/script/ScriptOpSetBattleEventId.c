// bdc 0x0881088c ScriptOpSetBattleEventId
#include "bdc.h"

/* Script opcode that arms an event battle: stores the u32 operand as the battle event id in script
   global 9 (`*0x08ac58c4 + 0x24`) and sets global 8 (`+0x20`, the game mode) to 1 = battle started
   from an event. An operand of -1 takes the current stage's default id from the table `0x08a348f8`
   (indexed by script global 1), or 100 when that entry is also -1. */

int ScriptOpSetBattleEventId(Script *script)

{
  u32 id;
  
  id = ScriptReadU32(script);
  if (id == 0xffffffff) {
    id = g_battleEventIdByStage[g_scriptGlobalVars[1]];
    if (id == 0xffffffff) {
      id = 100;
    }
  }
  g_scriptGlobalVars[9] = id;
  g_scriptGlobalVars[8] = 1;
  return 0;
}

