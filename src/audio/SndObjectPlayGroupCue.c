// bdc 0x089c20d4 SndObjectPlayGroupCue
#include "bdc.h"

/* Plays cue `cue` of sound group `group` on a `SndObject`, replacing whatever the object already
   plays from that group: it asks the `SndManager` for the slot of the loaded group
   (`SndManagerFindGroupSlot``(mgr, group, 1)`) and returns 0 if the group is not loaded
   (negative slot). Otherwise every emitter of the object whose `(u32)soundId >> 27` equals that
   slot is stopped (`repeat = 0`, `autoFree = released = 1`, array slot cleared) and a new emitter
   is added with `SndObjectAddEmitter``(obj, slot << 27 | cue | group << 20, loop, 0)`; its
   result is returned. */

bool SndObjectPlayGroupCue(SndObject *obj, s32 group, u32 cue, u8 loop)
{
  bool ok;
  SndManager *mgr;
  s32 slot;
  s32 i;
  SndEmitter *em;

  ok = false;
  mgr = SndGetManager();
  slot = SndManagerFindGroupSlot(mgr, group, true);
  if (slot >= 0) {
    for (i = 0; i < obj->emitterCount; i++) {
      em = obj->emitters[i];
      if (em != NULL && ((u32)em->soundId >> 27) == (u32)slot) {
        em->repeat = 0;
        obj->emitters[i]->autoFree = 1;
        obj->emitters[i]->released = 1;
        obj->emitters[i] = NULL;
      }
    }
    ok = SndObjectAddEmitter(obj, (s32)((u32)slot << 27 | cue | (u32)group << 20), loop, 0);
  }
  return ok;
}
