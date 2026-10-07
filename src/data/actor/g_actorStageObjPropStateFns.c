// bdc 0x08a850b8 g_actorStageObjPropStateFns
#include "bdc.h"

__typeof__(MemberFnPtr[4]) g_actorStageObjPropStateFns = {
    { .pfn = (void *)ActorStageObjPropState00Inactive },
    { .pfn = (void *)ActorStageObjPropState01Idle },
    { .pfn = (void *)ActorStageObjPropState02Knocked },
    { .pfn = (void *)ActorStageObjPropState03Topple },
};
