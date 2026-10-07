// bdc 0x089ca1d4 ScriptOpGetPadPrevButtons
#include "bdc.h"

/* Reads input into a script variable: stores `g_padState->prevButtons` (the button mask of the
   previous frame) into the output variable. */

int ScriptOpGetPadPrevButtons(Script *script)

{
  u16 prev;
  u32 *dst;
  
  prev = g_padState->prevButtons;
  dst = ScriptReadRef(script,2);
  *(u16 *)dst = prev;
  return 0;
}

