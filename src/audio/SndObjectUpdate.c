// bdc 0x089c2274 SndObjectUpdate
#include "bdc.h"

/* Per-frame update of one sound object (an owner of several `SndEmitter`s, items of the list kept
   in `g_soundObjectMgr`), called by `SndObjectMgrUpdate`; returns 0 when the object may be
   deleted. For an object whose `alive` byte (`+0x24`) is set it reaps finished emitters: every
   emitter that has no voice (`handle < 0`), no restarts left (`repeat == 0`) and no `autoFree` is
   destroyed with `SndEmitterDestroy` and its array slot cleared; it returns 1. For an object with
   `alive == 0` (being shut down) it first marks every playing emitter as released, detached (`src =
   NULL`) and `autoFree`; if none was playing it instead zeroes the `repeat` of every remaining
   emitter, flags them the same way and clears the array; it returns 1 if it touched anything this
   call and 0 once the array is empty. */

s32 SndObjectUpdate(void *object)

{
  SndObject *obj = (SndObject *)object;
  s32 result = 0;
  s32 i;

  if (obj->alive == 0) {
    for (i = 0; i < obj->emitterCount; i++) {
      SndEmitter *e = obj->emitters[i];
      if ((e != (SndEmitter *)0x0) && (e->handle >= 0)) {
        e->released = 1;
        result = 1;
        obj->emitters[i]->src = (float *)0x0;
        obj->emitters[i]->autoFree = 1;
      }
    }
    if (result == 0) {
      for (i = 0; i < obj->emitterCount; i++) {
        SndEmitter *e = obj->emitters[i];
        if (e != (SndEmitter *)0x0) {
          e->repeat = 0;
          obj->emitters[i]->autoFree = 1;
          result = 1;
          obj->emitters[i]->released = 1;
          obj->emitters[i] = (SndEmitter *)0x0;
        }
      }
    }
    return result;
  }

  for (i = 0; i < obj->emitterCount; i++) {
    SndEmitter *e = obj->emitters[i];
    if ((e != (SndEmitter *)0x0) && (e->handle < 0) && (e->repeat == 0) && (e->autoFree == 0)) {
      SndListener *listener = SndGetListener();
      if (SndEmitterDestroy(listener,obj->emitters[i])) {
        obj->emitters[i] = (SndEmitter *)0x0;
      }
    }
  }
  return 1;
}
