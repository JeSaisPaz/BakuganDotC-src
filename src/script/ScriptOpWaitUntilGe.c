// bdc 0x089caac8 ScriptOpWaitUntilGe
#include "bdc.h"

/* Blocks the track until `*var >= value` holds: if the condition is true the opcode completes,
   otherwise it ends the turn without advancing, so the same instruction is re-evaluated next frame.
    */

int ScriptOpWaitUntilGe(Script *script)

{
  u32 *ref;
  u32 value;
  int result;

  ref = ScriptReadRef(script,2);
  value = ScriptReadU32(script);
  result = 2;
  if ((int)value <= (int)*ref) {
    result = 0;
  }
  return result;
}
