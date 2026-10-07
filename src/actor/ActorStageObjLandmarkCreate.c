// bdc 0x088a1e50 ActorStageObjLandmarkCreate
#include "bdc.h"

/* Creates the landmark stage object (kind 0xb1 `ABYSS`, `f6_landmark01.gmo`): allocates 0x400 bytes
   from the low heap, runs `ActorStageObjLandmarkCtor`, sets HP and max HP (`+0x200/+0x204`) to
   800, creates its HP gauge (`ActorStageObjEnsureHpGauge`) and places the gauge 400 units above
   `pos` (`+0x2a0`; `pos[1]` itself is raised by 400). Called by `ActorStageObjCreateByKind`. */

void *ActorStageObjLandmarkCreate(float *pos)
{
  bool fromLow;
  ActorStageObjLandmark *mem;
  ActorStageObjLandmark *self = NULL;
  float local_pos[4] __attribute__((aligned(16)));

  MemLock();
  fromLow = MemIsAllocFromLow();
  MemSetAllocFromLow(true);
  mem = MemAlloc(0x400, NULL, 0);
  MemSetAllocFromLow(fromLow);
  MemUnlock();
  if (mem != NULL) {
    local_pos[0] = pos[0];
    local_pos[1] = pos[1];
    local_pos[2] = pos[2];
    local_pos[3] = pos[3];
    ActorStageObjLandmarkCtor(mem, local_pos);
    self = mem;
  }
  (self->base).maxHp = 800;
  (self->base).hp = 800;
  ActorStageObjEnsureHpGauge(&self->base);
  pos[1] = pos[1] + 400.0f;
  local_pos[0] = pos[0];
  local_pos[1] = pos[1];
  local_pos[2] = pos[2];
  local_pos[3] = pos[3];
  (self->base).gaugePos[0] = local_pos[0];
  (self->base).gaugePos[1] = local_pos[1];
  (self->base).gaugePos[2] = local_pos[2];
  (self->base).gaugePos[3] = local_pos[3];
  return self;
}
