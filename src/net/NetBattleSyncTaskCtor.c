// bdc 0x0881ce9c NetBattleSyncTaskCtor
#include "bdc.h"

/* Constructor of the netplay battle-settings sync task (task id 2001 = 0x7d1, 0x18 bytes, vtable
   `0x08af1694`): clears the step `+0x10` and the timeout counter `+0x14`. */

NetBattleSyncTask *NetBattleSyncTaskCtor(NetBattleSyncTask *self)

{
  CoreTaskInit(&self->base);
  (self->base).vtable = g_netBattleSyncTaskVtbl;
  self->step = 0;
  self->timeout = 0;
  return self;
}

