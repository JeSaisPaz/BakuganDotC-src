// bdc 0x089d0678 NetCharaClearLobbyFlag
#include "bdc.h"

/* Clears the character's header word `+0x34` (header `+0x28`, bit 0 is set by
   `NetCharaSetLobbyFlag`) under its lock. Called by `NetCharaCtor` and `NetCharaResetSync`.
    */

void NetCharaClearLobbyFlag(NetChara *self)

{
  CoreLock *lock;
  
  CoreLockAcquire(self->lock);
  lock = self->lock;
  (self->outHdr).flags = 0;
  CoreLockRelease(lock);
  return;
}

