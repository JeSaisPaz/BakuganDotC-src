// bdc 0x089c68d4 SndManagerIsSoundWordPlaying
#include "bdc.h"

/* Returns 1 if any of the 32 voices of the `SndManager` is live (`handle != 0`) and plays exactly
   `soundWord` (the sound word the voice was started with, `SndVoiceSlot.soundWord` at `+8`), else
   0. The scan runs under the manager lock. */

s32 SndManagerIsSoundWordPlaying(SndManager *mgr, u32 soundWord)
{
  s32 result = 0;
  int i;

  CoreLockAcquire(mgr->lock);
  for (i = 0; i < 32; i++) {
    if (mgr->voices[i].handle != 0 && mgr->voices[i].soundWord == soundWord) {
      result = 1;
      break;
    }
  }
  CoreLockRelease(mgr->lock);
  return result;
}
