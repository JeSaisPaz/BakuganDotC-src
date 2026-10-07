// bdc 0x089c006c SndListenerBind
#include "bdc.h"

/* Points the `SndListener` at live vectors instead of its inline copies: a non-NULL `pos`
   replaces `self->pos`, a non-NULL `target` replaces `self->target`; a NULL argument leaves that
   pointer unchanged. Unlike `SndListenerSet` nothing is copied, so the listener follows the
   vector owner (the camera or player object) every frame. */

void SndListenerBind(SndListener *self, float *pos, float *target)

{
  if (pos != (float *)0x0) {
    self->pos = pos;
  }
  if (target != (float *)0x0) {
    self->target = target;
  }
  return;
}

