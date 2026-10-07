// bdc 0x089c02b4 SndEmitterCreateLinked
#include "bdc.h"

/* Creates a positional `SndEmitter` whose position follows a live vector. Same allocation,
   zeroing and field setup as `SndEmitterCreateAtPos`, except that `src` is stored directly (the
   emitter reads three floats from it every frame, so it tracks the owner), `autoFree` is cleared,
   and the record is only inserted into `g_soundEmitterList` (priority `soundId`) when `addToList`
   is non-zero and the list exists. `params` comes from `SndEmitterGetProfile`, `handle = -1`,
   `repeat = -1` if `loop` is non-zero else 1. Returns the emitter, or NULL when the sound id has no
   profile. */

SndEmitter *SndEmitterCreateLinked(SndListener *listener, s32 soundId, float *src, u8 loop, u8 addToList)
{
  bool fromLow;
  SndEmitterProfile *profile;
  SndEmitter *emitter;

  emitter = NULL;
  profile = SndEmitterGetProfile(listener, soundId);
  if (profile != NULL) {
    if (g_soundEmitterPool == NULL) {
      emitter = NULL;
    }
    else {
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
    if (addToList != 0 && g_soundEmitterList != NULL) {
      SndEmitterListInsert(g_soundEmitterList, emitter, soundId);
    }
    emitter->soundId = soundId;
    emitter->handle = -1;
    emitter->src = src;
    emitter->params = profile;
    emitter->silenced = 0;
    emitter->released = 0;
    emitter->autoFree = 0;
    emitter->state38 = -1;
    emitter->groupCount = 0;
    emitter->targetVolume = 0.0f;
    emitter->repeat = loop != 0 ? -1 : 1;
  }
  return emitter;
}
