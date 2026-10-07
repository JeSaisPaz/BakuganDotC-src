// bdc 0x089c6548 SndManagerStop
#include "bdc.h"

/* Queues a stop request for voice `handle` on the sound manager: under the manager lock, if the
   state is 5 (ready) and a command slot is free, writes `cmdId = -1` and `cmdHandle = handle`
   there. Returns true if the request was queued, false if the manager is not ready or the queue is
   full. */

bool SndManagerStop(SndManager *mgr, s32 handle)
{
  s32 slot;

  CoreLockAcquire(mgr->lock);
  slot = -1;
  if (mgr->state == 5) {
    slot = SndManagerFindFreeSlot(mgr);
  }
  if (-1 < slot) {
    mgr->cmdId[slot] = -1;
    mgr->cmdHandle[slot] = handle;
  }
  CoreLockRelease(mgr->lock);
  return -1 < slot;
}
