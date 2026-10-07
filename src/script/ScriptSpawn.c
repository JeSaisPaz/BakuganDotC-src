// bdc 0x089cb98c ScriptSpawn
#include "bdc.h"

/* Creates and starts a script: allocates a 0x58-byte script object from the low end of the heap,
   runs `ScriptCtor` on it with the current list head `g_scriptList` as the anchor (the new
   node is linked behind it; NULL leaves it unlinked), loads the named script into it with
   `ScriptLoad` and, if the list was empty, makes it the list head. Returns the new script (or
   NULL if the allocation failed — note `ScriptLoad` is then still called with NULL). */

Script *ScriptSpawn(const char *name)
{
  bool fromLow;
  Script *alloc;
  Script *script;

  MemLock();
  fromLow = MemIsAllocFromLow();
  MemSetAllocFromLow(true);
  alloc = MemAlloc(sizeof(Script), NULL, 0);
  MemSetAllocFromLow(fromLow);
  MemUnlock();
  script = NULL;
  if (alloc != NULL) {
    ScriptCtor(alloc, (CoreNode *)g_scriptList);
    script = alloc;
  }
  ScriptLoad(script, name);
  if (g_scriptList == NULL) {
    g_scriptList = script;
  }
  return script;
}
