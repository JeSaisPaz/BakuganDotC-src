// bdc 0x08af2864 g_actorStageObjVtbl
#include "bdc.h"

__typeof__(VtblEntry[20]) g_actorStageObjVtbl = {
    {0}, { .fn = (void *)ActorStageObjDtor }, { .fn = (void *)GfxModelInitFields },
    { .fn = (void *)GfxModelSetFogEnabled }, { .fn = (void *)GfxModelSetFog },
    { .fn = (void *)GfxModelDistanceFade }, { .fn = (void *)GfxModelSetMotionSpeed },
    { .fn = (void *)ActorStageObjUpdateScripted }, { .fn = (void *)ActorStageObjDraw },
    { .fn = (void *)GfxModelSetToon }, { .fn = (void *)ActorStageObjBaseSetupMaterials },
    { .fn = (void *)ActorStageObjBaseBreak }, { .fn = (void *)ActorStageObjBaseIsAttrLandmark },
    { .fn = (void *)ActorStageObjIsTarget }, { .fn = (void *)ActorStageObjBaseIsEggCrystal },
    { .fn = (void *)ActorStageObjBaseIsCrystal },
    { .fn = (void *)ActorStageObjBaseSlot16ReturnFalse },
    { .fn = (void *)ActorStageObjBaseIsLandmark }, { .fn = (void *)ActorStageObjBaseHidesHpGauge },
    { .fn = (void *)ActorStageObjBaseReturnFalse },
};
