// bdc 0x089c000c SndListenerSet
#include "bdc.h"

/* Updates the 3D-sound `SndListener`: stores `heading` (radians, passed in the FPU register
   `f12`) at `+0x20` and, when `pos` / `target` are non-NULL, copies those three-float vectors into
   the listener's inline storage and re-points `listener->pos` / `listener->target` at the copies.
    */

void SndListenerSet(float heading, SndListener *listener, const float *pos, const float *target)

{
  if (pos != (float *)0x0) {
    listener->pos = listener->posData;
    listener->posData[0] = *pos;
    listener->pos[1] = pos[1];
    listener->pos[2] = pos[2];
  }
  if (target != (float *)0x0) {
    listener->target = listener->targetData;
    listener->targetData[0] = *target;
    listener->target[1] = target[1];
    listener->target[2] = target[2];
  }
  listener->heading = heading;
  return;
}

