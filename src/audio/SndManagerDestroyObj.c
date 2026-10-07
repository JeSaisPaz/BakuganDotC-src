// bdc 0x089c5aa0 SndManagerDestroyObj
#include "bdc.h"

/* Destructor of `SndManager`: unregisters the power callbacks (`SndManagerOnSuspend` with
   `CorePowerRemoveSuspendCallback`, `SndManagerOnResume` with `CorePowerRemoveResumeCallback` on
   `CorePowerGet`) and, when `flags & 1`, frees the object with `MemFree` under the heap lock.
    */

void SndManagerDestroyObj(SndManager *mgr, u32 flags)
{
  if (mgr != (SndManager *)0x0) {
    CorePowerRemoveSuspendCallback(CorePowerGet(), SndManagerOnSuspend);
    CorePowerRemoveResumeCallback(CorePowerGet(), SndManagerOnResume);
    if ((flags & 1) != 0) {
      MemLock();
      MemFree(mgr, (char *)0x0, 0);
      MemUnlock();
    }
  }
}
