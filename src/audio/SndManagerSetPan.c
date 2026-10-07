// bdc 0x089c66cc SndManagerSetPan
#include "bdc.h"

/* Queues a set-pan command for voice `handle`: `cmdId = -5`, `cmdHandle = handle`, `cmdParam =
   clamp((int)(pan * 64) + 0x40, 0, 0x7f)`, i.e. pan -1..1 mapped to the 7-bit range with 0x40 as
   centre. Does nothing unless the manager is ready (state 5) and a command slot is free. */

void SndManagerSetPan(float pan, SndManager *mgr, s32 handle)

{
  s32 slot = -1;
  s32 param;

  CoreLockAcquire(mgr->lock);
  if (mgr->state == 5) {
    slot = SndManagerFindFreeSlot(mgr);
  }
  if (slot >= 0) {
    mgr->cmdId[slot] = -5;
    param = (s32)(pan * 64.0f) + 0x40;
    if (param < 0) {
      param = 0;
    } else if (param > 0x7f) {
      param = 0x7f;
    }
    mgr->cmdParam[slot] = param;
    mgr->cmdHandle[slot] = handle;
  }
  CoreLockRelease(mgr->lock);
}
