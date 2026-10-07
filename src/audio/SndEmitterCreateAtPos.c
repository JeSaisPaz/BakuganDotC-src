// bdc 0x089c0084 SndEmitterCreateAtPos
#include "bdc.h"

/* Creates a positional `SndEmitter` at a fixed world position and registers it in
   `g_soundEmitterList`. The record comes from `g_soundEmitterPool` (`MemPoolAlloc`) or, when
   the pool is empty or missing, from the low heap (0x48 bytes); it is zeroed, inserted into the
   list with priority `soundId` (`SndEmitterListInsert`) and initialised: `soundId`, `handle = -1`
   (not playing), `params` = `SndEmitterGetProfile``(soundId)`, the three floats of `pos` copied
   into `posA` with `src` pointing at that inline copy (a frozen position), `volume = pan = 0`,
   `silenced = released = 0`, `autoFree` from the argument, `state38 = -1`, `groupCount = 0`,
   `targetVolume = 0` and `repeat = -1` (loop until stopped) when `loop` is non-zero, else 1.
   Returns the new emitter (the `NULL` result for a missing profile is dead code: the lookup always
   returns a row); `SndEmitterUpdateAll` later starts and stops the voice by distance to the
   `SndListener`. */

SndEmitter *SndEmitterCreateAtPos(SndListener *listener, s32 soundId, float *pos, u8 loop, u8 autoFree)
{
  bool fromLow;
  SndEmitterProfile *profile;
  SndEmitter *emitter;

  emitter = NULL;
  profile = SndEmitterGetProfile(listener, soundId);
  if (profile != NULL) {
    if (g_soundEmitterPool == NULL) {
      emitter = NULL;
    } else {
      emitter = MemPoolAlloc(g_soundEmitterPool);
    }
    if (emitter == NULL) {
      MemLock();
      fromLow = MemIsAllocFromLow();
      MemSetAllocFromLow(true);
      emitter = MemAlloc(sizeof(SndEmitter), NULL, 0);
      MemSetAllocFromLow(fromLow);
      MemUnlock();
    }
    memset(emitter, 0, sizeof(SndEmitter));
    SndEmitterListInsert(g_soundEmitterList, emitter, soundId);
    emitter->soundId = soundId;
    emitter->handle = -1;
    emitter->src = emitter->posA;
    emitter->params = profile;
    emitter->posA[0] = pos[0];
    emitter->posA[1] = pos[1];
    emitter->posA[2] = pos[2];
    emitter->volume = 0.0f;
    emitter->pan = 0.0f;
    emitter->silenced = 0;
    emitter->released = 0;
    emitter->autoFree = autoFree;
    emitter->state38 = -1;
    emitter->groupCount = 0;
    emitter->targetVolume = 0.0f;
    emitter->repeat = (loop != 0) ? -1 : 1;
  }
  return emitter;
}
