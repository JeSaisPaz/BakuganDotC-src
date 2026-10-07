// bdc 0x08a83eb8 g_actorStageObjMineStateFns
#include "bdc.h"

__typeof__(MemberFnPtr[2]) g_actorStageObjMineStateFns = {
    { .pfn = (void *)ActorStageObjMineState00Armed },
    { .pfn = (void *)ActorStageObjMineState01Triggered },
};
