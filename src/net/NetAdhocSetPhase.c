// bdc 0x089d4a70 NetAdhocSetPhase
#include "bdc.h"

/* Stores `phase` (0..6; other values are ignored) into the connection object's phase field
   (`+0x48`) under its lock (`+0x30`). Phases 3 and 4 also set the byte `+0x1f`. Initialised to 0 by
   `NetAdhocConnCtor`; `NetPlayStateAbort`'s adhoc steps move it to 5 (`NetAdhocBeginStop`, `NetAdhocStopLink`)
   or 2 (`NetAdhocStopLink` when the adhocctl state is 3). */

void NetAdhocSetPhase(NetAdhocConn *self, s32 phase)

{
  if ((-1 < phase) && (phase < 7)) {
    CoreLockAcquire(self->lock);
    self->phase = phase;
    if ((2 < phase) && (phase < 5)) {
      self->phaseActive = '\x01';
    }
    CoreLockRelease(self->lock);
  }
  return;
}

