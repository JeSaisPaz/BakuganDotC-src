// bdc 0x088a6c3c ActorStageObjAttrLandmarkClearAuraFlags
#include "bdc.h"

/* Clears the 7 per-status 'aura held' flags `0x08abd53c[0..6]` used by
   `ActorStageObjAttrLandmarkApplyAura`. Called by `ActorStageObjAttrLandmarkCtor` and
   `ActorStageObjUpdateAll` (battle/field setup). */

void ActorStageObjAttrLandmarkClearAuraFlags(void)

{
  int i;

  for (i = 0; i < 7; i++) {
    g_stageObjAuraHeld[i] = 0;
  }
}
