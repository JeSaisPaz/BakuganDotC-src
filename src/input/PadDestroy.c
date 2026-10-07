// bdc 0x089ce7fc PadDestroy
#include "bdc.h"

/* Destroys the main controller object: `PadDtor`(g_padState, 3) and sets `g_padState` to NULL.
   No-op if it does not exist. */

void PadDestroy(void)

{
  if (g_padState != (PadState *)0x0) {
    PadDtor(g_padState,3);
    g_padState = (PadState *)0x0;
  }
  return;
}

