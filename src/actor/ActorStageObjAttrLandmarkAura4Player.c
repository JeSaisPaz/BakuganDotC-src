// bdc 0x088a7b94 ActorStageObjAttrLandmarkAura4Player
#include "bdc.h"

/* Aura handler for aura type 4: entry 5 of `g_actorStageObjAttrLandmarkAuraFns` (pfn at
   `0x08a83f6c`; entry 0 is `ActorStageObjAttrLandmarkAuraNone`, so entry = type + 1), run by
   `ActorStageObjAttrLandmarkUpdate`. Applies the aura to the player Bakugan only
   (`BtlGetPlayerBakugan`, `ActorStageObjAttrLandmarkApplyAura`). */

void ActorStageObjAttrLandmarkAura4Player(ActorStageObjAttrLandmark *self)

{
  void *unit;
  
  unit = BtlGetPlayerBakugan();
  ActorStageObjAttrLandmarkApplyAura(self,unit);
  return;
}

