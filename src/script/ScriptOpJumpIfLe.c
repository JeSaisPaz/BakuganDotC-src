// bdc 0x089ca654 ScriptOpJumpIfLe
#include "bdc.h"

/* Conditional jump: compares a script variable with a value (signed 32-bit) and, if `*var <=
   value`, jumps to the target (`curTrack->pc = target`); otherwise falls through. */

int ScriptOpJumpIfLe(Script *script)

{
  u32 *var;
  u32 value;
  u32 target;
  
  var = ScriptReadRef(script,2);
  value = ScriptReadU32(script);
  target = ScriptReadU16(script);
  if ((int)*var <= (int)value) {
    script->curTrack->pc = (u16)target;
    return 3;
  }
  return 0;
}

