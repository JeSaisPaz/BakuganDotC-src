// bdc 0x08a83f40 g_actorStageObjAttrLandmarkAuraFns
#include "bdc.h"

__typeof__(MemberFnPtr[7]) g_actorStageObjAttrLandmarkAuraFns = {
    { .pfn = (void *)ActorStageObjAttrLandmarkAuraNone },
    { .pfn = (void *)ActorStageObjAttrLandmarkAura0Player },
    { .pfn = (void *)ActorStageObjAttrLandmarkAura1Player },
    { .pfn = (void *)ActorStageObjAttrLandmarkAura2Player },
    { .pfn = (void *)ActorStageObjAttrLandmarkAura3AllUnits },
    { .pfn = (void *)ActorStageObjAttrLandmarkAura4Player },
    { .pfn = (void *)ActorStageObjAttrLandmarkAura5Player },
};
