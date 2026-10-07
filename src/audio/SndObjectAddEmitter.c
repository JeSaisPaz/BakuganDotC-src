// bdc 0x089c1f4c SndObjectAddEmitter
#include "bdc.h"

/* Starts a sound on a `SndObject`: scans the emitter array for the first free (NULL) slot and,
   with `skipIfPlaying` non-zero, for an existing emitter with the same `soundId`. When the array
   exists, there is a free slot and no duplicate was found it creates a position-tracking emitter
   with `SndEmitterCreateLinked``(listener, soundId, obj->src, loop, 1)` (listener from
   `SndGetListener`), stores it in the free slot and returns 1; otherwise returns 0 (no array,
   array full or duplicate). */

bool SndObjectAddEmitter(SndObject *obj, s32 soundId, u8 loop, u8 skipIfPlaying)
{
  bool found = false;
  bool added = false;
  SndEmitter **freeSlot = NULL;
  SndEmitter **slots = obj->emitters;
  SndEmitter **p;
  int i;

  if (slots != NULL) {
    p = slots;
    for (i = 0; i < obj->emitterCount; i++, p++) {
      if (*p == NULL) {
        if (freeSlot == NULL) {
          freeSlot = p;
        }
      } else if (skipIfPlaying != 0 && (*p)->soundId == soundId) {
        found = true;
      }
    }
    if (!found && freeSlot != NULL) {
      SndListener *listener = SndGetListener();
      *freeSlot = SndEmitterCreateLinked(listener, soundId, obj->src, loop, 1);
      added = true;
    }
  }
  return added;
}
