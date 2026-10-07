// bdc 0x08850e08 BtlIsStoryBattle
#include "bdc.h"

/* True when the battle rule mode selector (script global variable 8) is 1 and the current stage
   number (script global variable 1) is not 0x24. */
bool BtlIsStoryBattle(void)
{
    return g_scriptGlobalVars[8] == 1 && g_scriptGlobalVars[1] != 0x24;
}
