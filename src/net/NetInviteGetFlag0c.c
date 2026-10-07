// bdc 0x089d2668 NetInviteGetFlag0c
#include "bdc.h"

/* Returns the byte at `+0xc` of the invite object (`g_netInvite`) under its lock. Used by
   `NetPlayState1Connect`; the flag's meaning is not established (positional name). */

u8 NetInviteGetFlag0c(NetInvite *self)

{
  u8 flag;
  
  CoreLockAcquire(self->lock);
  flag = self->flag0c;
  CoreLockRelease(self->lock);
  return flag;
}

