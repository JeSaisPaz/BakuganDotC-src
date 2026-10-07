// bdc 0x089d41a4 NetAdhocGetPhase
#include "bdc.h"

/* Returns the connection object's phase `+0x48` (see `NetAdhocSetPhase`) read under its lock. */

s32 NetAdhocGetPhase(NetAdhocConn *self)
{
  s32 phase;

  CoreLockAcquire(self->lock);
  phase = self->phase;
  CoreLockRelease(self->lock);
  return phase;
}
