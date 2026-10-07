// bdc 0x089bffbc SndListenerInit
#include "bdc.h"

/* Constructor of the `SndListener`: points `pos` at the inline `posData` and `target` at the
   inline `targetData`, zeroes both inline vectors, the `heading` (+0x20) and the muted flag
   `flag24` (+0x24), and returns `self`. */

SndListener *SndListenerInit(SndListener *self)
{
  self->pos = self->posData;
  self->target = self->targetData;
  self->posData[2] = 0.0f;
  self->pos[1] = 0.0f;
  self->pos[0] = 0.0f;
  self->target[2] = 0.0f;
  self->target[1] = 0.0f;
  self->target[0] = 0.0f;
  self->heading = 0.0f;
  self->flag24 = 0;
  return self;
}
