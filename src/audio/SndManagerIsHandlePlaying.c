// bdc 0x089c6850 SndManagerIsHandlePlaying
#include "bdc.h"

/* Returns 1 if voice `handle` is currently in the manager's voice table (any of the 32
   `SndVoiceSlot`s has `handle` equal to it), else 0. Scans under the manager lock. */

bool SndManagerIsHandlePlaying(SndManager *mgr, s32 handle)
{
  bool found = false;
  int i;

  CoreLockAcquire(mgr->lock);
  for (i = 0; i < 0x20; i++) {
    if (mgr->voices[i].handle == handle) {
      found = true;
      break;
    }
  }
  CoreLockRelease(mgr->lock);
  return found;
}
