// bdc 0x089ffe08 ScriptDtor
#include "bdc.h"

/* Destructor of the concrete script class (vtable `0x08af59fc` entry 1): installs its own vtable,
   runs `ScriptBaseDtor` with `flags = 0` and frees the script when bit 0 of `flags` is set. */

void ScriptDtor(Script *script, u32 flags)

{
  if (script != (Script *)0x0) {
    script->vtable = g_scriptVtbl;
    ScriptBaseDtor(script,0);
    if ((flags & 1) != 0) {
      MemLock();
      MemFree(script,(char *)0x0,0);
      MemUnlock();
    }
  }
  return;
}

