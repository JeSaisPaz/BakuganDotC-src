// bdc 0x088e0414 ActorSpawnFromPlacement
#include "bdc.h"

/* Spawns the actor of a field placement record: position = record ints x 20/4096, model from the
   record's character code `+0x36` (`ActorModelIdFromCode`); the edit-man (0x2f) spawns with flag
   0, others with 3 (`ActorSpawn`). Stores the slot `+0x34c` and the record `+0x350`, plays the
   placed idle motion, applies the placement and scales Bakugan models (< 0x21) by 10. Returns the
   actor. */

void *ActorSpawnFromPlacement(u8 slot, s32 *record)
{
  GameFieldPlacedChar *placed = (GameFieldPlacedChar *)record;
  float pos[4] __attribute__((aligned(16)));
  Actor *self;
  u32 modelId;

  pos[0] = (float)placed->pos[0] * 0.00024414062f;
  pos[1] = (float)placed->pos[1] * 0.00024414062f;
  pos[2] = (float)placed->pos[2] * 0.00024414062f;
  pos[3] = 0.0f;
  /* vscl.t + sv.q: xyz scaled by 20; the stored w lane is a stale VFPU value (ActorSpawn reads
     only xyz), left out. */
  pos[0] = pos[0] * 20.0f;
  pos[1] = pos[1] * 20.0f;
  pos[2] = pos[2] * 20.0f;
  modelId = ActorModelIdFromCode(placed->modelCode);
  /* a3 = record is also passed in both calls; ActorSpawn never reads it. */
  if (modelId == 0x2f) {
    self = ActorSpawn(modelId, 0, pos);
  }
  else {
    self = ActorSpawn(modelId, 3, pos);
  }
  self->placementSlot = slot;
  self->placement = placed;
  ActorPlayPlacedMotion(self, 1);
  ActorApplyPlacement(self);
  if ((s32)modelId < 0x21) {
    ActorMulScale(10.0f, self);
  }
  return self;
}
