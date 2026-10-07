// bdc 0x08a15c70 GmoMotionGetUser
#include "bdc.h"

/* Returns the float user word `+0x1c` of a motion record (the current frame), or 0.0f for NULL. */

float GmoMotionGetUser(GmoMotionRecord *motion)
{
  if (motion == (GmoMotionRecord *)0x0) {
    return 0.0f;
  }
  return motion->frame;
}
