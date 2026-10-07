// bdc 0x088a7aec ActorStageObjAttrLandmarkAura2Player
#include "bdc.h"

/* Aura handler for type 2 (entry 2 of the member-pointer table `0x08a83f44`, run by
   `ActorStageObjAttrLandmarkUpdate`): applies the aura to the player Bakugan only
   (`BtlGetPlayerBakugan`, `ActorStageObjAttrLandmarkApplyAura`). */

void ActorStageObjAttrLandmarkAura2Player(ActorStageObjAttrLandmark *self)

{
  void *unit;
  
  unit = BtlGetPlayerBakugan();
  ActorStageObjAttrLandmarkApplyAura(self,unit);
  return;
}

