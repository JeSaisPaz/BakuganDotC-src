// bdc 0x089ca20c ScriptOpGetPadButtons
#include "bdc.h"

/* Reads input into a script variable: stores `g_padState->buttons` (the current button mask) into
   the output variable. */

int ScriptOpGetPadButtons(Script *script)

{
  u16 value;
  u32 *dst;
  
  value = g_padState->buttons;
  dst = ScriptReadRef(script,2);
  *(u16 *)dst = value;
  return 0;
}

