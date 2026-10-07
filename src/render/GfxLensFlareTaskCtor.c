// bdc 0x088a1348 GfxLensFlareTaskCtor
#include "bdc.h"

/* Constructor of the lens-flare task (task id 482 = 0x1e2, 0x60 bytes, vtable `0x08af23fc`; killed
   by `BtlMainTaskDtor`): clears its state and the sun direction vector `+0x30`, then fills that
   vector from the field (`GameStageGetSunPosition`) or battle (`BtlStageGetLensFlarePos`) stage
   depending on `GameFieldTaskExists`; the returned value, stored as a byte at `+0x10`, enables the
   effect. Returns `task`. */

CoreTask *GfxLensFlareTaskCtor(CoreTask *task)
{
  GfxLensFlareTask *self = (GfxLensFlareTask *)task;
  s32 ok;

  CoreTaskInit(task);
  task->vtable = g_gfxLensFlareTaskVtbl;
  self->unk14 = 0;
  self->unk18 = 0.0f;
  self->unk1c = 0;
  self->layer = 0;
  /* sv.q of bank C720 = (0, 0, 0, 0) */
  self->sunPos[0] = 0.0f;
  self->sunPos[1] = 0.0f;
  self->sunPos[2] = 0.0f;
  self->sunPos[3] = 0.0f;
  if (GameFieldTaskExists() == 0) {
    ok = BtlStageGetLensFlarePos(self->sunPos);
  } else {
    ok = GameStageGetSunPosition(self->sunPos);
  }
  self->enabled = (u8)ok;
  return task;
}
