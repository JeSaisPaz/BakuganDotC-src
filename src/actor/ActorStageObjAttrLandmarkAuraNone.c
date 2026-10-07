// bdc 0x088a79b8 ActorStageObjAttrLandmarkAuraNone
#include "bdc.h"

/* Empty aura handler for attribute index 0 (entry 0 of the aura member-pointer table run by
   ActorStageObjAttrLandmarkUpdate): a landmark without an attribute applies no aura. */
void ActorStageObjAttrLandmarkAuraNone(ActorStageObjAttrLandmark *self)
{
    (void)self;
}
