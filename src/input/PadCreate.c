// bdc 0x089ce778 PadCreate
#include "bdc.h"

/* Creates the main controller object: allocates 0x5c bytes from the low end of the heap (under
   `MemLock`), runs `PadCtor` on it and stores the result in `g_padState` (NULL if the
   allocation failed). */

void PadCreate(void)

{
  bool fromLow;
  PadState *pad;
  PadState *result;

  MemLock();
  fromLow = MemIsAllocFromLow();
  MemSetAllocFromLow(true);
  pad = MemAlloc(0x5c,(char *)0x0,0);
  MemSetAllocFromLow(fromLow);
  MemUnlock();
  result = (PadState *)0x0;
  if (pad != (PadState *)0x0) {
    PadCtor(pad);
    result = pad;
  }
  g_padState = result;
  return;
}
