// bdc 0x089ca2fc ScriptOpGetPadStickX
#include "bdc.h"

/* Reads input into a script variable: stores `g_padState->stickX` (the analogue stick X axis
   (float)) into the output variable. */

int ScriptOpGetPadStickX(Script *script)

{
  float *dst;
  float x;
  
  x = g_padState->stickX;
  dst = (float *)ScriptReadRef(script,2);
  *dst = x;
  return 0;
}

