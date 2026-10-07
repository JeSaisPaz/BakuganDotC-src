// bdc 0x0882b9a8 ActorCrystalSpawnTaskDtor
#include "bdc.h"

/* Destructor of the crystal spawn task (id 105, `ActorCrystalSpawnTaskCtor`): clears the
   spawn-request byte `g_actorCrystalSpawnRequest`, chains to `CoreTaskDestroy`, frees on bit 0
   of `flags`. */

void ActorCrystalSpawnTaskDtor(CoreTask *task, u32 flags)

{
  if (task != NULL) {
    task->vtable = g_actorCrystalSpawnTaskVtable;
    g_actorCrystalSpawnRequest = 0;
    CoreTaskDestroy(task, 0);
    if ((flags & 1) != 0) {
      MemLock();
      MemFree(task, NULL, 0);
      MemUnlock();
    }
  }
}
