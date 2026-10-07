// bdc 0x089c9168 ScriptBaseDtor
#include "bdc.h"

/* Destructor of the script base class (`g_scriptBaseVtbl` entry 1): resets the vtable pointer,
   frees the decoded package entry (`entry`), the track states (`tracks`), the variable table
   (`vars`), the flag-bit table (`flagBits`) and the track-local block (`trackLocals`) under
   `MemLock`, writes 2 into the parent's child-state byte (`*script->parentState`, if any),
   releases the sound object (`soundObj`) with `SndObjectRelease`, runs `CoreNodeDtor` and
   frees the script itself when bit 0 of `flags` is set. */

void ScriptBaseDtor(Script *script, u32 flags)
{
  if (script == NULL) {
    return;
  }
  script->vtable = g_scriptBaseVtbl;
  if (script->entry != NULL) {
    void *ptr = script->entry;
    MemLock();
    MemFree(ptr, NULL, 0);
    MemUnlock();
    script->entry = NULL;
  }
  if (script->tracks != NULL) {
    ScriptTrack *ptr = script->tracks;
    MemLock();
    MemFree(ptr, NULL, 0);
    MemUnlock();
    script->tracks = NULL;
  }
  if (script->vars != NULL) {
    u32 *ptr = script->vars;
    MemLock();
    MemFree(ptr, NULL, 0);
    MemUnlock();
    script->vars = NULL;
  }
  if (script->flagBits != NULL) {
    u32 *ptr = script->flagBits;
    MemLock();
    MemFree(ptr, NULL, 0);
    MemUnlock();
    script->flagBits = NULL;
  }
  if (script->trackLocals != NULL) {
    ScriptTrackLocal *ptr = script->trackLocals;
    MemLock();
    MemFree(ptr, NULL, 0);
    MemUnlock();
    script->trackLocals = NULL;
  }
  if (script->parentState != NULL) {
    *script->parentState = 2;
  }
  SndObjectRelease(SndGetObjectList(), script->soundObj);
  CoreNodeDtor((CoreNode *)script, 0);
  if ((flags & 1) != 0) {
    MemLock();
    MemFree(script, NULL, 0);
    MemUnlock();
  }
}
