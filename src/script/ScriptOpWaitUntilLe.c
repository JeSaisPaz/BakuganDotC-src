// bdc 0x089cab14 ScriptOpWaitUntilLe
#include "bdc.h"

/* Blocks the track until `*var <= value` holds (signed compare): if the condition is true the opcode
   completes (returns 0), otherwise it returns 2 and ends the turn without advancing, so the same
   instruction is re-evaluated next frame. */

int ScriptOpWaitUntilLe(Script *script)

{
  u32 *var;
  u32 value;
  int result;

  var = ScriptReadRef(script,2);
  value = ScriptReadU32(script);
  result = 2;
  if ((int)*var <= (int)value) {
    result = 0;
  }
  return result;
}
