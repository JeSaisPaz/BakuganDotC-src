// bdc 0x0881cedc NetBattleSyncTaskDtor
#include "bdc.h"

/* Destructor of the netplay battle-settings sync task (id 2001): restores the vtable, chains to
   `CoreTaskDestroy`, frees on bit 0 of `flags`. */

void NetBattleSyncTaskDtor(NetBattleSyncTask *self, u32 flags)

{
  if (self != (NetBattleSyncTask *)0x0) {
    (self->base).vtable = g_netBattleSyncTaskVtbl;
    CoreTaskDestroy(&self->base,0);
    if ((flags & 1) != 0) {
      MemLock();
      MemFree(self,(char *)0x0,0);
      MemUnlock();
    }
  }
  return;
}

