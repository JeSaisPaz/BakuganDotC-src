// bdc 0x089ca334 ScriptOpGetPadStickY
#include "bdc.h"

/* Reads input into a script variable: stores `g_padState->stickY` (the analogue stick Y axis
   (float)) into the output variable. */

int ScriptOpGetPadStickY(Script *script)

{
  float *dst;
  float y;
  
  y = g_padState->stickY;
  dst = (float *)ScriptReadRef(script,2);
  *dst = y;
  return 0;
}

