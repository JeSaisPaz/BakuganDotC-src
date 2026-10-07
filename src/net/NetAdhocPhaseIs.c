// bdc 0x089d4a18 NetAdhocPhaseIs
#include "bdc.h"

/* Returns whether the connection object's phase field (`+0x48`, set by `NetAdhocSetPhase`) equals
   `phase`, read under the object's lock (`+0x30`). Three callers, among them the abort steps used
   by `NetPlayStateAbort` (`NetAdhocBeginStop`, `NetAdhocIsDisconnected`). */

bool NetAdhocPhaseIs(NetAdhocConn *self, s32 phase)

{
  s32 cur;
  
  CoreLockAcquire(self->lock);
  cur = self->phase;
  CoreLockRelease(self->lock);
  return cur == phase;
}

