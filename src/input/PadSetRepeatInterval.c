// bdc 0x089ce174 PadSetRepeatInterval
#include "bdc.h"

/* Sets the number of frames between auto-repeat triggers (`PadState` `repeatInterval`, offset
   0x2e) and returns the previous value. */

u16 PadSetRepeatInterval(PadState *pad, u16 interval)
{
  u16 old;

  old = pad->repeatInterval;
  pad->repeatInterval = interval;
  return old;
}
