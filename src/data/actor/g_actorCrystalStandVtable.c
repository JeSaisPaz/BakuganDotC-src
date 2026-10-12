// bdc 0x08af24d4 g_actorCrystalStandVtable
#include "bdc.h"

__typeof__(VtblEntry[10]) g_actorCrystalStandVtable = {
    {0}, { .fn = (void *)ActorCrystalStandDtor }, { .fn = (void *)GfxModelInitFields },
    { .fn = (void *)GfxModelSetFogEnabled }, { .fn = (void *)GfxModelSetFog },
    { .fn = (void *)GfxModelDistanceFade }, { .fn = (void *)GfxModelSetMotionSpeed },
    { .fn = (void *)ActorCrystalStandUpdate }, { .fn = (void *)ActorCrystalStandDraw },
    { .fn = (void *)GfxModelSetToon },
};
