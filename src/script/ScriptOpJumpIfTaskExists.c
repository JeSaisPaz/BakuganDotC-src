// bdc 0x089caff4 ScriptOpJumpIfTaskExists
#include "bdc.h"

/* Conditional jump: jumps to the target if a live task with the given id is in the task list
   (`CoreTaskExists`). */

int ScriptOpJumpIfTaskExists(Script *script)

{
  u32 id;
  u32 target;
  s32 exists;
  
  id = ScriptReadU16(script);
  target = ScriptReadU16(script);
  exists = CoreTaskExists(id);
  if (exists != 0) {
    script->curTrack->pc = (u16)target;
    return 3;
  }
  return 0;
}

