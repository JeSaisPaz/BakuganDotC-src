// bdc 0x08a15c54 GmoMotionGetData
#include "bdc.h"

/* Returns `motion + 0x10` (the embedded data area of a 0x30-byte motion record), or NULL. */

void *GmoMotionGetData(GmoMotionRecord *motion)
{
  if (motion == (GmoMotionRecord *)0x0) {
    return (void *)0x0;
  }
  return motion->data;
}
