// bdc 0x089ca284 ScriptOpGetPadReleased
#include "bdc.h"

/* Reads input into a script variable: stores `g_padState->released` (the buttons released this
   frame) into the output variable. */

int ScriptOpGetPadReleased(Script *script)

{
  u16 value;
  u32 *dst;
  
  value = g_padState->released;
  dst = ScriptReadRef(script,2);
  *(u16 *)dst = value;
  return 0;
}

