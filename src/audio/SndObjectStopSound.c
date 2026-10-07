// bdc 0x089c203c SndObjectStopSound
#include "bdc.h"

/* Stops the sounds of a `SndObject`: every emitter in its array whose `soundId` equals the
   argument (every emitter when `soundId == -1`) gets `repeat = 0`, `autoFree = 1` and `released =
   1` and is removed from the object's array. `SndEmitterUpdateAll` then lets the voice finish and
   frees the record. Returns 1 if at least one emitter was stopped, else 0. */

bool SndObjectStopSound(SndObject *obj, s32 soundId)
{
  bool stopped = false;
  int i;

  for (i = 0; i < obj->emitterCount; i++) {
    SndEmitter *e = obj->emitters[i];
    if (e != NULL && (soundId == -1 || e->soundId == soundId)) {
      e->repeat = 0;
      obj->emitters[i]->autoFree = 1;
      stopped = true;
      obj->emitters[i]->released = 1;
      obj->emitters[i] = NULL;
    }
  }
  return stopped;
}
