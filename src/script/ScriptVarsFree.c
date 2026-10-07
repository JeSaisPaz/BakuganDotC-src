// bdc 0x089c9ab8 ScriptVarsFree
#include "bdc.h"

/* Frees the script variable block through `g_scriptGlobalVars` (the saved base of
   `g_scriptVars`) under `MemLock` and clears that pointer. */

void ScriptVarsFree(void)

{
  if (g_scriptGlobalVars != (s32 *)0x0) {
    MemLock();
    MemFree(g_scriptGlobalVars,(char *)0x0,0);
    MemUnlock();
    g_scriptGlobalVars = (s32 *)0x0;
  }
  return;
}

