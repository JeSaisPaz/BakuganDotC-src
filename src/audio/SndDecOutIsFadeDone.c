// bdc 0x089c4eb8 SndDecOutIsFadeDone
#include "bdc.h"

/* Tests the fade bookkeeping under the lock: with `silent == 0` returns 1 when `volFrom == volTo`
   (no fade in progress); with `silent != 0` returns 1 when `volFrom == 0` (the channel is, or has
   finished fading to, silence); else 0. */

s32 SndDecOutIsFadeDone(SndDecOut *dec, u8 silent)
{
  s32 done = 0;

  CoreLockAcquire(dec->lock);
  if (silent == 0) {
    if (dec->volFrom == dec->volTo) {
      done = 1;
    }
  } else if (dec->volFrom == 0) {
    done = 1;
  }
  CoreLockRelease(dec->lock);
  return done;
}
