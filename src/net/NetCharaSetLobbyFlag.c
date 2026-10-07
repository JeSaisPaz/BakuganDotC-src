// bdc 0x089d06ac NetCharaSetLobbyFlag
#include "bdc.h"

/* Sets the character's header word `+0x34` to 1 under its lock; `UiNetLobbyHostPhase` sets it on
   the local character, so it travels to the peers in the header. The exact meaning (e.g.
   "hosting/recruiting") is [INFERENCE]. */

void NetCharaSetLobbyFlag(NetChara *self)

{
  CoreLock *lock;
  
  CoreLockAcquire(self->lock);
  lock = self->lock;
  (self->outHdr).flags = 1;
  CoreLockRelease(lock);
  return;
}

