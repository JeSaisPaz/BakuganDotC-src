// bdc 0x08a85278 g_actorStageObjCrystalStateFns
#include "bdc.h"

__typeof__(MemberFnPtr[2]) g_actorStageObjCrystalStateFns = {
    { .pfn = (void *)ActorStageObjCrystalState00Idle },
    { .pfn = (void *)ActorStageObjCrystalState01Shoot },
};
