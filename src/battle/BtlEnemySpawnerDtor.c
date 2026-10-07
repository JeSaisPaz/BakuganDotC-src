// bdc 0x088a5590 BtlEnemySpawnerDtor
#include "bdc.h"

/* Deleting destructor of the enemy spawner (entry 1 of `g_btlEnemySpawnerVtbl`): re-installs the
   spawner vtable, runs the `CoreObject` base destructor `CoreObjectDtor` without freeing, then
   frees the object under `MemLock` when bit 0 of `flags` is set. Does nothing for NULL. */
void BtlEnemySpawnerDtor(void *spawner, u32 flags)
{
  CoreObject *obj = (CoreObject *)spawner;

  if (obj != NULL) {
    obj->vtable = g_btlEnemySpawnerVtbl;
    CoreObjectDtor(obj, 0);
    if ((flags & 1) != 0) {
      MemLock();
      MemFree(obj, NULL, 0);
      MemUnlock();
    }
  }
}
