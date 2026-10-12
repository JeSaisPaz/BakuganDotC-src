// bdc 0x08af37e4 g_actorVtbl
#include "bdc.h"

__typeof__(VtblEntry[32]) g_actorVtbl = {
    {0}, { .fn = (void *)ActorDtor }, { .fn = (void *)GfxModelInitFields },
    { .fn = (void *)GfxModelSetFogEnabled }, { .fn = (void *)GfxModelSetFog },
    { .fn = (void *)ActorUpdateDistanceFade }, { .fn = (void *)GfxModelSetMotionSpeed },
    { .fn = (void *)ActorUpdate }, { .fn = (void *)ActorDraw }, { .fn = (void *)GfxModelSetToon },
    { .fn = (void *)ActorUpdateMatrix }, { .fn = (void *)ActorIsBallTarget },
    { .fn = (void *)ActorIsRobot }, { .fn = (void *)ActorResetBehaviour },
    { .fn = (void *)ActorSlot14Nop }, { .fn = (void *)ActorSlot15 },
    { .fn = (void *)ActorEnterSavePlacementState }, { .fn = (void *)ActorHearNoiseNop },
    { .fn = (void *)ActorWalkToward }, { .fn = (void *)ActorIsSpecialRoute },
    { .fn = (void *)ActorStateIdle }, { .fn = (void *)ActorState01Nop },
    { .fn = (void *)ActorState02Nop }, { .fn = (void *)ActorState03Nop },
    { .fn = (void *)ActorState04Nop }, { .fn = (void *)ActorState05Nop },
    { .fn = (void *)ActorState06Nop }, { .fn = (void *)ActorState07Nop },
    { .fn = (void *)ActorStateRestorePlacement }, { .fn = (void *)ActorStateRouteWalk },
    { .fn = (void *)ActorStateRouteNextStep }, { .fn = (void *)ActorRunState },
};
