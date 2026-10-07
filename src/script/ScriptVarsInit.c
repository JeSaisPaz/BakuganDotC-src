// bdc 0x089c99d8 ScriptVarsInit
#include "bdc.h"

/* Creates the script variable block: frees any previous `g_scriptVars`, allocates a zeroed
   0xfd0-byte block from the top of the heap, records its base in `g_scriptGlobalVars` and `base +
   0x80` in `g_scriptGlobalBits`, and re-points the player profile at `base + 0x100`
   (`SaveProfileAttachBlock`) before rebuilding the profile with `SaveProfileInit`. */

void ScriptVarsInit(void)

{
  bool fromLow;
  void *s;
  
  if (g_scriptVars != (void *)0x0) {
    MemLock();
    MemFree(g_scriptVars,(char *)0x0,0);
    MemUnlock();
    g_scriptVars = (void *)0x0;
  }
  MemLock();
  fromLow = MemIsAllocFromLow();
  MemSetAllocFromLow(false);
  s = MemAlloc(0x100 /* PSP: global var table (0x80) + global flag bits (0x80) */ + sizeof(SaveProfileData),(char *)0x0,0);
  MemSetAllocFromLow(fromLow);
  MemUnlock();
  g_scriptVars = s;
  memset(s,0,0x100 /* PSP: global var table (0x80) + global flag bits (0x80) */ + sizeof(SaveProfileData));
  g_scriptGlobalVars = g_scriptVars;
  g_scriptGlobalBits = (u32 *)&g_scriptGlobalVars[32];
  SaveProfileAttachBlock(g_scriptVars);
  SaveProfileInit();
  return;
}

