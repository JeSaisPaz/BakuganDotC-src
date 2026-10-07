// bdc 0x089c6480 SndManagerIsSoundPlaying
#include "bdc.h"

/* Returns 1 if the sound effect `(groupId, soundIdx)` currently occupies a voice of the
   `SndManager`, else 0. Under the manager lock it requires the manager to be ready (`state == 5`)
   and a free command-queue slot (`SndManagerFindFreeSlot`; the same guard `SndManagerPlay`
   uses, so while the queue is full or the manager is not ready the answer is 0), finds the
   sound-bank slot loaded for `groupId` (`SndManagerFindGroupSlot`) and then checks the voices for
   the composed sound word `slot << 27 | groupId << 20 | soundIdx`
   (`SndManagerIsSoundWordPlaying`). */

s32 SndManagerIsSoundPlaying(SndManager *mgr, s32 groupId, u32 soundIdx)

{
  s32 result;
  s32 slot;
  
  result = 0;
  slot = -1;
  CoreLockAcquire(mgr->lock);
  if (mgr->state == 5) {
    slot = SndManagerFindFreeSlot(mgr);
  }
  if (-1 < slot) {
    slot = SndManagerFindGroupSlot(mgr,groupId,false);
    if (-1 < slot) {
      result = SndManagerIsSoundWordPlaying(mgr,slot << 0x1b | soundIdx | groupId << 0x14);
    }
  }
  CoreLockRelease(mgr->lock);
  return result;
}

