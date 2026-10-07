// bdc 0x089d2cac NetInviteClearPendingB
#include "bdc.h"

/* Clears the invite object's second pending flag (byte `+5`) under its lock. */

void NetInviteClearPendingB(NetInvite *self)

{
  CoreLockAcquire(self->lock);
  self->pendingB = '\0';
  CoreLockRelease(self->lock);
  return;
}

