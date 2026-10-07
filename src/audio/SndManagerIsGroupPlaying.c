// bdc 0x089c67ac SndManagerIsGroupPlaying
#include "bdc.h"

/* Returns 1 if any live voice belongs to sound group `group`: a `SndVoiceSlot` with non-zero
   `handle` whose `soundWord >> 20 & 0x7f` equals `group`. Always 0 for `group == 0xffffffff`. Scans
   under the manager lock. */

bool SndManagerIsGroupPlaying(SndManager *mgr, u32 group)
{
  bool result = false;
  int i;

  if (group != 0xffffffff) {
    CoreLockAcquire(mgr->lock);
    for (i = 0; i < 32; i++) {
      if (mgr->voices[i].handle != 0 &&
          group == ((int)mgr->voices[i].soundWord >> 0x14 & 0x7fU)) {
        result = true;
        break;
      }
    }
    CoreLockRelease(mgr->lock);
  }
  return result;
}
