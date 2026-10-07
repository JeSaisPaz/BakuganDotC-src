// bdc 0x089c0404 SndEmitterDestroy
#include "bdc.h"

/* Destroys a positional sound emitter: if it has a playing voice (`handle >= 0`) and the sound
   manager exists, a stop command is queued with `SndManagerStop`; once that succeeds the emitter
   is removed from `g_soundEmitterList` (`SndEmitterListRemove`) and its record is returned to
   `g_soundEmitterPool` (`MemPoolFree`) or the heap. Returns 1 when done and 0 when the emitter
   could not be destroyed yet (the stop command could not be queued, or the emitter is still flagged
   `autoFree` but was not in the list; it is then left allocated). */

bool SndEmitterDestroy(SndListener *listener, SndEmitter *emitter)
{
  s32 ok;

  ok = 1;
  if (emitter->handle >= 0 && SndHasManager()) {
    ok = SndManagerStop(SndGetManager(), emitter->handle);
  }
  if (ok != 0) {
    if (SndEmitterListRemove(g_soundEmitterList, emitter)) {
      if (g_soundEmitterPool != NULL && MemPoolFree(g_soundEmitterPool, emitter)) {
        emitter = NULL;
      }
      if (emitter != NULL) {
        MemLock();
        MemFree(emitter, NULL, 0);
        MemUnlock();
      }
    }
    else if (emitter->autoFree == 0) {
      if (g_soundEmitterPool != NULL && MemPoolFree(g_soundEmitterPool, emitter)) {
        emitter = NULL;
      }
      if (emitter != NULL) {
        MemLock();
        MemFree(emitter, NULL, 0);
        MemUnlock();
      }
    }
    else {
      ok = 0;
    }
  }
  return ok;
}
