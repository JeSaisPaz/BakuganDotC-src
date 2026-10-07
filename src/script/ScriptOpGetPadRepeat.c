// bdc 0x089ca2c0 ScriptOpGetPadRepeat
#include "bdc.h"

/* Reads input into a script variable: stores `g_padState->repeat` (the key-repeat button mask) into
   the output variable. */

int ScriptOpGetPadRepeat(Script *script)

{
  u16 value;
  u32 *dst;
  
  value = g_padState->repeat;
  dst = ScriptReadRef(script,2);
  *(u16 *)dst = value;
  return 0;
}

