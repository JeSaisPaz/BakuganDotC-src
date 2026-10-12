// bdc 0x08af38e4 g_actorPlayerVtbl
#include "bdc.h"

__typeof__(VtblEntry[32]) g_actorPlayerVtbl = {
    {0}, { .fn = (void *)ActorPlayerDtor }, { .fn = (void *)GfxModelInitFields },
    { .fn = (void *)GfxModelSetFogEnabled }, { .fn = (void *)GfxModelSetFog },
    { .fn = (void *)ActorUpdateDistanceFade }, { .fn = (void *)GfxModelSetMotionSpeed },
    { .fn = (void *)ActorPlayerUpdate }, { .fn = (void *)ActorPlayerDraw },
    { .fn = (void *)GfxModelSetToon }, { .fn = (void *)ActorPlayerUpdateMatrix },
    { .fn = (void *)ActorIsBallTarget }, { .fn = (void *)ActorIsRobot },
    { .fn = (void *)ActorPlayerResetBehaviour }, { .fn = (void *)ActorSlot14Nop },
    { .fn = (void *)ActorSlot15 }, { .fn = (void *)ActorEnterSavePlacementState },
    { .fn = (void *)ActorHearNoiseNop }, { .fn = (void *)ActorWalkToward },
    { .fn = (void *)ActorIsSpecialRoute }, { .fn = (void *)ActorPlayerStateIdle },
    { .fn = (void *)ActorPlayerStateWalk }, { .fn = (void *)ActorPlayerState02Nop },
    { .fn = (void *)ActorPlayerState03Nop }, { .fn = (void *)ActorPlayerStateUseTerminal },
    { .fn = (void *)ActorPlayerStateStepFollowCamUntilCircle },
    { .fn = (void *)ActorPlayerStateWaitStepFollowCam }, { .fn = (void *)ActorPlayerStateThrow },
    { .fn = (void *)ActorPlayerStateRestorePlacement }, { .fn = (void *)ActorStateRouteWalk },
    { .fn = (void *)ActorStateRouteNextStep }, { .fn = (void *)ActorPlayerRunState },
};
