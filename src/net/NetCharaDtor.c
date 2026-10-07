// bdc 0x089d0394 NetCharaDtor
#include "bdc.h"

/* Destructor of a `CONetChara`: unlinks it from the `g_netCharaMgr` list (under the manager
   lock), frees the outgoing (`outBuf`) and receive (`recvBuf`) buffers, destroys its `CoreLock`
   (`lock`) and frees the object when `flags & 1`. NULL `self` does nothing. */

void NetCharaDtor(NetChara *self, u32 flags)
{
  if (self == NULL) {
    return;
  }
  CoreLockAcquire(g_netCharaMgr->lock);
  NetCharaListRemove((CoreList *)g_netCharaMgr->list, self);
  CoreLockRelease(g_netCharaMgr->lock);
  if (self->outBuf != NULL) {
    MemLock();
    MemFree(self->outBuf, NULL, 0);
    MemUnlock();
    self->outBuf = NULL;
  }
  if (self->recvBuf != NULL) {
    MemLock();
    MemFree(self->recvBuf, NULL, 0);
    MemUnlock();
    self->recvBuf = NULL;
  }
  if (self->lock != NULL) {
    CoreLockDestroy(self->lock, 3);
    self->lock = NULL;
  }
  if ((flags & 1) != 0) {
    MemLock();
    MemFree(self, NULL, 0);
    MemUnlock();
  }
}
