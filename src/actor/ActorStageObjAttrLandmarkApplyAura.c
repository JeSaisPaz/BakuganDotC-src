// bdc 0x088a7a0c ActorStageObjAttrLandmarkApplyAura
#include "bdc.h"

/* Applies the landmark's aura to `unit` while the landmark stands: inside the radius
   (`ActorStageObjAttrLandmarkInRange`) sets the held flag `g_stageObjAuraHeld``[type]` and gives the unit
   status `type + 0xb` (`BtlCombatApplyAuraStatus` on its `BtlCombatState` `+0x434`); outside, clears that
   status (`BtlCombatClearStatus`) unless another landmark of the same type holds it this frame.
   `type` = `+0x328`. */

void ActorStageObjAttrLandmarkApplyAura(ActorStageObjAttrLandmark *self, void *unit)

{
  int inRange;

  if (((self->base).dead == '\0') && (unit != (void *)0x0)) {
    inRange = ActorStageObjAttrLandmarkInRange(self,unit);
    if (inRange != 0) {
      g_stageObjAuraHeld[self->auraType] = 1;
      BtlCombatApplyAuraStatus(&((BtlBakugan *)unit)->combat,self->auraType + 0xb);
      return;
    }
    if (g_stageObjAuraHeld[self->auraType] == 0) {
      BtlCombatClearStatus(&((BtlBakugan *)unit)->combat,self->auraType + 0xb);
    }
  }
  return;
}
