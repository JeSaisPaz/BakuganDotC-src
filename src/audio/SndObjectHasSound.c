// bdc 0x089c2434 SndObjectHasSound
#include "bdc.h"

/* Returns 1 if the `SndObject` has an emitter in its array whose `soundId` equals the argument,
   else 0 (also 0 when the array is NULL). Stopped emitters no longer count, since
   `SndObjectStopSound` clears their slot. */

bool SndObjectHasSound(SndObject *obj, s32 soundId)
{
  SndEmitter **p = obj->emitters;
  int i;

  if (p != NULL) {
    for (i = 0; i < obj->emitterCount; i++, p++) {
      if (*p != NULL && (*p)->soundId == soundId) {
        return true;
      }
    }
  }
  return false;
}
