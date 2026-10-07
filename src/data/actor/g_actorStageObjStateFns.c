// bdc 0x08a842f8 g_actorStageObjStateFns
#include "bdc.h"

__typeof__(MemberFnPtr[12]) g_actorStageObjStateFns = {
    { .pfn = (void *)ActorStageObjState00Update }, { .pfn = (void *)ActorStageObjState01Nop },
    { .pfn = (void *)ActorStageObjState02Update }, { .pfn = (void *)ActorStageObjState03Nop },
    { .pfn = (void *)ActorStageObjState04Update }, { .pfn = (void *)ActorStageObjState05Update },
    { .pfn = (void *)ActorStageObjState06Update }, { .pfn = (void *)ActorStageObjState07Update },
    { .pfn = (void *)ActorStageObjState08Update }, { .pfn = (void *)ActorStageObjState09Update },
    { .pfn = (void *)ActorStageObjState10FadeIn }, { .pfn = (void *)ActorStageObjState11FadeOut },
};
