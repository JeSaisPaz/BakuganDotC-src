// bdc 0x08af27b4 g_actorStageObjAttrLandmarkVtbl
#include "bdc.h"

__typeof__(VtblEntry[20]) g_actorStageObjAttrLandmarkVtbl = {
    {0}, { .fn = (void *)ActorStageObjAttrLandmarkDtor }, { .fn = (void *)GfxModelInitFields },
    { .fn = (void *)GfxModelSetFogEnabled }, { .fn = (void *)GfxModelSetFog },
    { .fn = (void *)GfxModelDistanceFade }, { .fn = (void *)GfxModelSetMotionSpeed },
    { .fn = (void *)ActorStageObjAttrLandmarkUpdate },
    { .fn = (void *)ActorStageObjAttrLandmarkDraw }, { .fn = (void *)GfxModelSetToon },
    { .fn = (void *)ActorStageObjBaseSetupMaterials }, { .fn = (void *)ActorStageObjBaseBreak },
    { .fn = (void *)ActorStageObjAttrLandmarkIsAttrLandmark },
    { .fn = (void *)ActorStageObjBaseIsTarget }, { .fn = (void *)ActorStageObjBaseIsEggCrystal },
    { .fn = (void *)ActorStageObjBaseIsCrystal },
    { .fn = (void *)ActorStageObjBaseSlot16ReturnFalse },
    { .fn = (void *)ActorStageObjBaseIsLandmark }, { .fn = (void *)ActorStageObjBaseHidesHpGauge },
    { .fn = (void *)ActorStageObjBaseReturnFalse },
};
