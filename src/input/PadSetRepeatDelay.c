// bdc 0x089ce164 PadSetRepeatDelay
#include "bdc.h"

/* Sets the number of frames a button must be held before auto-repeat begins (`PadState`
   `repeatDelay`, offset 0x2c) and returns the previous value. */

u16 PadSetRepeatDelay(PadState *pad, u16 delay)
{
  u16 old;

  old = pad->repeatDelay;
  pad->repeatDelay = delay;
  return old;
}
