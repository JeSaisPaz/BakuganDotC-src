// bdc 0x089c6178 SndManagerFindGroupSlot
#include "bdc.h"

/* Returns the index of the group slot of the `SndManager` whose `name` equals the file name of
   group `groupId` (`strcmp` against the seGRP table entry; the last matching slot wins), or -1 if
   none matches. The lock is only taken when `lock` is non-zero. */

s32 SndManagerFindGroupSlot(SndManager *mgr, s32 groupId, bool lock)

{
  const char *name = g_soundGroupNames[groupId];
  s32 result = -1;
  s32 i;

  if (lock) {
    CoreLockAcquire(mgr->lock);
  }
  for (i = 0; i < 32; i++) {
    if (mgr->groups[i].name != NULL && strcmp(mgr->groups[i].name, name) == 0) {
      result = i;
    }
  }
  if (lock) {
    CoreLockRelease(mgr->lock);
  }
  return result;
}
