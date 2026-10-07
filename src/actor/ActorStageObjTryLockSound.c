// bdc 0x088ac96c ActorStageObjTryLockSound
#include "bdc.h"

/* Per-sound rate limiter: if the byte `0x08b00ac0[id]` is 0, sets it to 10 (frames) and returns 1,
   else returns 0. Called by the debris model update `ActorStageObjDebrisUpdate` before playing
   impact sounds. */

int ActorStageObjTryLockSound(int id)

{
  if (g_stageObjSoundLock[id] == 0) {
    g_stageObjSoundLock[id] = 10;
    return 1;
  }
  return 0;
}
