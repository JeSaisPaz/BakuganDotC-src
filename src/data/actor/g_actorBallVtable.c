// bdc 0x08af2c94 g_actorBallVtable
#include "bdc.h"

__typeof__(VtblEntry[10]) g_actorBallVtable = {
    {0}, { .fn = (void *)ActorBallDtor }, { .fn = (void *)GfxModelInitFields },
    { .fn = (void *)GfxModelSetFogEnabled }, { .fn = (void *)GfxModelSetFog },
    { .fn = (void *)GfxModelDistanceFade }, { .fn = (void *)GfxModelSetMotionSpeed },
    { .fn = (void *)ActorBallUpdate }, { .fn = (void *)ActorBallDraw },
    { .fn = (void *)GfxModelSetToon },
};
