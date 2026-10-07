// bdc 0x088a7bc4 ActorStageObjAttrLandmarkAura5Player
#include "bdc.h"

/* Aura handler for type 5 (entry 5 of the member-pointer table `0x08a83f44`, run by
   `ActorStageObjAttrLandmarkUpdate`): applies the aura to the player Bakugan only
   (`BtlGetPlayerBakugan`, `ActorStageObjAttrLandmarkApplyAura`). */

void ActorStageObjAttrLandmarkAura5Player(ActorStageObjAttrLandmark *self)

{
  void *unit;
  
  unit = BtlGetPlayerBakugan();
  ActorStageObjAttrLandmarkApplyAura(self,unit);
  return;
}

