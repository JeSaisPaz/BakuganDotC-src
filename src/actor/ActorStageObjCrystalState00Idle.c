// bdc 0x088b4f60 ActorStageObjCrystalState00Idle
#include "bdc.h"

/* State 0 (idle) of the crystal stage object (table `0x08a85278`): unless a cut-in runs
   (`BtlIsCutInRunning`), the crystal is inactive (`+0x395` clear) or the global
   `g_btlControlLockAll` is set, counts down `+0x33c`; at 0 it reloads it with 90..239 frames
   (`CoreRandNext``(150) + 90`) and switches to state 1 (`ActorStageObjCrystalSetState`). */

void ActorStageObjCrystalState00Idle(ActorStageObjCrystal *self)
{
  if (!BtlIsCutInRunning() && self->active != 0 && g_btlControlLockAll == 0) {
    self->idleTimer = self->idleTimer - 1;
    if (self->idleTimer < 1) {
      self->idleTimer = CoreRandNext(0x96) + 0x5a;
      ActorStageObjCrystalSetState(self, 1);
    }
  }
}
