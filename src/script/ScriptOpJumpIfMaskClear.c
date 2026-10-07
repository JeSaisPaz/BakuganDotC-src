// bdc 0x089cb094 ScriptOpJumpIfMaskClear
#include "bdc.h"

/* Conditional jump on a bit mask: jumps when `(*var & mask) == 0`. */

int ScriptOpJumpIfMaskClear(Script *script)

{
  u32 *var;
  u32 mask;
  u32 target;
  
  var = ScriptReadRef(script,2);
  mask = ScriptReadU32(script);
  target = ScriptReadU16(script);
  if ((*var & mask) == 0) {
    script->curTrack->pc = (u16)target;
    return 3;
  }
  return 0;
}

