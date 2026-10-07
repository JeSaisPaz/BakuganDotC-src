// bdc 0x08a83e20 g_actorStageObjEggCrystalStateFns
#include "bdc.h"

__typeof__(MemberFnPtr[3]) g_actorStageObjEggCrystalStateFns = {
    { .pfn = (void *)ActorStageObjEggCrystalState00Idle },
    { .pfn = (void *)ActorStageObjEggCrystalState01Appear },
    { .pfn = (void *)ActorStageObjEggCrystalState02Hatch },
};
