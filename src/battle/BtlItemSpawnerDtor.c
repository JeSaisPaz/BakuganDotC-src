// bdc 0x088a82fc BtlItemSpawnerDtor
#include "bdc.h"

/* Deleting destructor of the battle item spawner (entry 1 of `g_btlItemSpawnerVtbl`): re-installs that vtable, runs the
   `CoreObject` base destructor `CoreObjectDtor` without freeing, then frees the object under
   `MemLock` when bit 0 of `flags` is set. Does nothing for NULL. */
void BtlItemSpawnerDtor(void *spawner, u32 flags)
{
  CoreObject *obj = (CoreObject *)spawner;

  if (obj != NULL) {
    obj->vtable = g_btlItemSpawnerVtbl;
    CoreObjectDtor(obj, 0);
    if ((flags & 1) != 0) {
      MemLock();
      MemFree(obj, NULL, 0);
      MemUnlock();
    }
  }
}
