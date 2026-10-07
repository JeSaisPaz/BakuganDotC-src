// bdc 0x089c21f4 SndObjectStopGroupCue
#include "bdc.h"

/* Stops cue `cue` of sound group `group` on a `SndObject`: looks up the slot of the loaded group
   with `SndManagerFindGroupSlot``(mgr, group, 1)`, returns 0 if it is not loaded, otherwise
   returns `SndObjectStopSound``(obj, slot << 27 | group << 20 | cue)`. */

bool SndObjectStopGroupCue(SndObject *obj, s32 group, u32 cue)

{
  bool result = false;
  SndManager *mgr = SndGetManager();
  s32 slot = SndManagerFindGroupSlot(mgr,group,true);

  if (-1 < slot) {
    result = SndObjectStopSound(obj,slot << 0x1b | cue | group << 0x14);
  }
  return result;
}
