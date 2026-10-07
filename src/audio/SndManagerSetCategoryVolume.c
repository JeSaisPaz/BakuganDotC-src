// bdc 0x089c5c48 SndManagerSetCategoryVolume
#include "bdc.h"

/* Stores `volume` for sound category `category` (1..4; anything else is ignored) in the manager's
   five-entry float pair table: `*(float *)(mgr + 0x1ac + category * 8) = volume`, under the manager
   lock. */

void SndManagerSetCategoryVolume(float volume, SndManager *mgr, s32 category)

{
  CoreLock *lock;
  
  if ((0 < category) && (category < 5)) {
    CoreLockAcquire(mgr->lock);
    lock = mgr->lock;
    mgr->catVolume[category][1] = volume;
    CoreLockRelease(lock);
  }
  return;
}

