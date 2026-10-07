// bdc 0x089c65dc SndManagerSetVolume
#include "bdc.h"

/* Queues a set-volume command for voice `handle`: `cmdId = -4`, `cmdHandle = handle`, `cmdParam =
   clamp(volume * master, 0, master)` where `master` is `SndManagerGetMasterVolume`. Does nothing
   unless the manager is ready (state 5) and a command slot is free. */

void SndManagerSetVolume(float volume, SndManager *mgr, s32 handle)

{
  s32 slot;
  s32 param;

  slot = -1;
  CoreLockAcquire(mgr->lock);
  if (mgr->state == 5) {
    slot = SndManagerFindFreeSlot(mgr);
  }
  if (slot >= 0) {
    mgr->cmdId[slot] = -4;
    param = (s32)((float)SndManagerGetMasterVolume(mgr) * volume);
    if (param < 0) {
      param = 0;
    } else if (SndManagerGetMasterVolume(mgr) < param) {
      param = SndManagerGetMasterVolume(mgr);
    }
    mgr->cmdParam[slot] = param;
    mgr->cmdHandle[slot] = handle;
  }
  CoreLockRelease(mgr->lock);
}
