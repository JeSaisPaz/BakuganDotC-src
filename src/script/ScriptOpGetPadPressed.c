// bdc 0x089ca248 ScriptOpGetPadPressed
#include "bdc.h"

/* Reads input into a script variable: stores `g_padState->pressed` (the buttons newly pressed this
   frame) into the output variable. */

int ScriptOpGetPadPressed(Script *script)

{
  u16 value;
  u32 *dst;
  
  value = g_padState->pressed;
  dst = ScriptReadRef(script,2);
  *(u16 *)dst = value;
  return 0;
}

