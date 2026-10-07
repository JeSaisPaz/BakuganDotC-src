// bdc 0x089c5ed8 SndManagerQueryGroupLoad
#include "bdc.h"

/* Queries the loading state of sound groups under the manager lock. With `groupId == -1` returns 1
   when no in-use slot (`state != 0`) is still waiting for its bank (`bankId == -1`), i.e. all
   requested groups finished loading, else 0. With a real `groupId` returns 1 if an in-use slot with
   that `groupId` exists, else 0. */

bool SndManagerQueryGroupLoad(SndManager *mgr, s32 groupId)

{
  bool result = true;
  s32 i;

  CoreLockAcquire(mgr->lock);
  if (groupId == -1) {
    for (i = 0; i < 32; i++) {
      if (mgr->groups[i].state != 0 && mgr->groups[i].bankId == -1) {
        result = false;
        break;
      }
    }
  } else {
    result = false;
    for (i = 0; i < 32; i++) {
      if (mgr->groups[i].state != 0 && mgr->groups[i].groupId == groupId) {
        result = true;
        break;
      }
    }
  }
  CoreLockRelease(mgr->lock);
  return result;
}
