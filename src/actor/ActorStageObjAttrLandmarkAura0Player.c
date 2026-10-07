// bdc 0x088a7a8c ActorStageObjAttrLandmarkAura0Player
#include "bdc.h"

/* Aura handler for type 0 (entry 0 of the member-pointer table `0x08a83f44`, run by
   `ActorStageObjAttrLandmarkUpdate`): applies the aura to the player Bakugan only
   (`BtlGetPlayerBakugan`, `ActorStageObjAttrLandmarkApplyAura`). */

void ActorStageObjAttrLandmarkAura0Player(ActorStageObjAttrLandmark *self)

{
  void *unit;
  
  unit = BtlGetPlayerBakugan();
  ActorStageObjAttrLandmarkApplyAura(self,unit);
  return;
}

