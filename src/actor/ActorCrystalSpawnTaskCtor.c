// bdc 0x0882b960 ActorCrystalSpawnTaskCtor
#include "bdc.h"

/* Constructor of the crystal spawn task, id 105 (0x69, 0x18 bytes, vtable `0x08af1754`): clears its
   step and the spawn-request byte `0x08ab9f70`. Once that byte is set the task spawns one crystal
   at a random free point and a crystal stand (`fz_crystal01_stand_60.gmo`,
   `ActorCrystalStandSpawnAtPoint`) on every crystal point of the stage
   (`ActorCrystalSpawnTaskStep`). */

CoreTask *ActorCrystalSpawnTaskCtor(CoreTask *task)

{
  CoreTaskInit(task);
  task->vtable = g_actorCrystalSpawnTaskVtable;
  ((ActorCrystalSpawnTask *)task)->step = 0;
  ((ActorCrystalSpawnTask *)task)->timer = 0;
  g_actorCrystalSpawnRequest = '\0';
  return task;
}

