// bdc 0x089c0530 SndListenerSetMuted
#include "bdc.h"

/* Writes the listener's `flag24` byte (+0x24): setter of the flag that `SndListenerIsMuted`
   returns. While it is non-zero `SndEmitterUpdateAll` stops the voice of every emitter, so the
   positional sound effects are silenced without destroying the emitters. */

void SndListenerSetMuted(SndListener *self, u8 muted)

{
  self->flag24 = muted;
  return;
}

