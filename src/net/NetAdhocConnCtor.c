// bdc 0x089d3ad8 NetAdhocConnCtor
#include "bdc.h"

/* Constructor of the 0x5c-byte `COPSPNet` ad-hoc connection object: allocates (low heap) and
   creates its LwMutex `CoreLock` `"COPSPNet"` (NULL if the allocation fails), clears the event
   state (`NetAdhocClearEvents`), sets handlerId = -1, clears the flag bytes, sets link state 0
   (`NetAdhocSetLinkState`) and phase 0 (`NetAdhocSetPhase`), copies `"GAME"` into the
   manager's gameTag, zeroes phase/ctlState/step/linkedRole, sets retriesLeft = 3 and clears
   `g_netAdhocThreadExit`. `mode` and `role` are left untouched. Returns `self`. */

NetAdhocConn *NetAdhocConnCtor(NetAdhocConn *self)
{
  bool fromLow;
  CoreLock *lock;

  MemLock();
  fromLow = MemIsAllocFromLow();
  MemSetAllocFromLow(true);
  lock = MemAlloc(sizeof(CoreLock), NULL, 0);
  MemSetAllocFromLow(fromLow);
  MemUnlock();
  if (lock != NULL) {
    CoreLockInit(lock, "COPSPNet", CORE_LOCK_LWMUTEX);
  }
  self->lock = lock;
  NetAdhocClearEvents(self);
  self->handlerId = -1;
  self->flag1c = 0;
  self->flag1d = 0;
  self->stopRequested = 0;
  self->phaseActive = 0;
  NetAdhocSetLinkState(self, 0);
  NetAdhocSetPhase(self, 0);
  strcpy(g_netAdhoc->gameTag, "GAME");
  self->flag20 = 0;
  self->phase = 0;
  self->ctlState = 0;
  self->step = 0;
  self->linkedRole = 0;
  self->retriesLeft = 3;
  g_netAdhocThreadExit = false;
  self->linkShown = 0;
  self->isHost = false;
  self->peerConfirmed = 0;
  return self;
}
