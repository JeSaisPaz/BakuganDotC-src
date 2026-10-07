// bdc 0x08a15c60 GmoMotionSetUser
#include "bdc.h"

/* Stores the float `value` in the user word `+0x1c` of a motion record (the current frame); no-op for NULL. */

void GmoMotionSetUser(GmoMotionRecord *motion, float value)
{
  if (motion != (GmoMotionRecord *)0x0) {
    motion->frame = value;
  }
}
