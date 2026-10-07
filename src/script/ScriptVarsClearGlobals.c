// bdc 0x089c96b4 ScriptVarsClearGlobals
#include "bdc.h"

/* Zeroes both 0x80-byte script global-variable areas (`*0x08ac58c4` and `*0x08ac58c8`, the first
   0x100 bytes of the save/script block) when `g_scriptVars` exists. Called by `UiTitleCtor`
   when the title screen starts, so a new game begins with cleared script globals. */

void ScriptVarsClearGlobals(void)

{
  if (g_scriptVars != (void *)0x0) {
    memset(g_scriptGlobalVars,0,0x80);
    memset(g_scriptGlobalBits,0,0x80);
  }
  return;
}

