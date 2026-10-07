// bdc 0x089cab60 ScriptOpWaitUntilNe
#include "bdc.h"

/* Blocks the track until `*var != value` holds: if the condition is true the opcode completes,
   otherwise it ends the turn without advancing, so the same instruction is re-evaluated next frame.
    */

int ScriptOpWaitUntilNe(Script *script)

{
  u32 *var;
  u32 value;
  int result;
  
  var = ScriptReadRef(script,2);
  value = ScriptReadU32(script);
  result = 2;
  if (*var != value) {
    result = 0;
  }
  return result;
}

