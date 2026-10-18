// bdc 0x088a7bc4 ActorStageObjAttrLandmarkAura5Player
#include "bdc.h"

/* Aura handler for aura type 5: entry 6 of `g_actorStageObjAttrLandmarkAuraFns` (pfn at
   `0x08a83f74`; entry 0 is `ActorStageObjAttrLandmarkAuraNone`, so entry = type + 1), run by
   `ActorStageObjAttrLandmarkUpdate`. Applies the aura to the player Bakugan only
   (`BtlGetPlayerBakugan`, `ActorStageObjAttrLandmarkApplyAura`). */

void ActorStageObjAttrLandmarkAura5Player(ActorStageObjAttrLandmark *self)

{
  void *unit;
  
  unit = BtlGetPlayerBakugan();
  ActorStageObjAttrLandmarkApplyAura(self,unit);
  return;
}

