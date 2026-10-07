// bdc 0x088a7abc ActorStageObjAttrLandmarkAura1Player
#include "bdc.h"

/* Aura handler for type 1 (entry 1 of the member-pointer table `0x08a83f44`, run by
   `ActorStageObjAttrLandmarkUpdate`): applies the aura to the player Bakugan only
   (`BtlGetPlayerBakugan`, `ActorStageObjAttrLandmarkApplyAura`). */

void ActorStageObjAttrLandmarkAura1Player(ActorStageObjAttrLandmark *self)

{
  void *unit;
  
  unit = BtlGetPlayerBakugan();
  ActorStageObjAttrLandmarkApplyAura(self,unit);
  return;
}

