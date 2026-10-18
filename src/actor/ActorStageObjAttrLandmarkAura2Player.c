// bdc 0x088a7aec ActorStageObjAttrLandmarkAura2Player
#include "bdc.h"

/* Aura handler for aura type 2: entry 3 of `g_actorStageObjAttrLandmarkAuraFns` (pfn at
   `0x08a83f5c`; entry 0 is `ActorStageObjAttrLandmarkAuraNone`, so entry = type + 1), run by
   `ActorStageObjAttrLandmarkUpdate`. Applies the aura to the player Bakugan only
   (`BtlGetPlayerBakugan`, `ActorStageObjAttrLandmarkApplyAura`). */

void ActorStageObjAttrLandmarkAura2Player(ActorStageObjAttrLandmark *self)

{
  void *unit;
  
  unit = BtlGetPlayerBakugan();
  ActorStageObjAttrLandmarkApplyAura(self,unit);
  return;
}

