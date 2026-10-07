// bdc 0x08af2434 g_actorStageObjLandmarkVtbl
#include "bdc.h"

__typeof__(VtblEntry[20]) g_actorStageObjLandmarkVtbl = {
    {0}, { .fn = (void *)ActorStageObjLandmarkDtor }, { .fn = (void *)GfxModelInitFields },
    { .fn = (void *)GfxModelSetFogEnabled }, { .fn = (void *)GfxModelSetFog },
    { .fn = (void *)GfxModelDistanceFade }, { .fn = (void *)GfxModelSetMotionSpeed },
    { .fn = (void *)ActorStageObjLandmarkUpdate }, { .fn = (void *)ActorStageObjLandmarkDraw },
    { .fn = (void *)GfxModelSetToon }, { .fn = (void *)ActorStageObjLandmarkSetupMaterials },
    { .fn = (void *)ActorStageObjLandmarkBreak }, { .fn = (void *)ActorStageObjBaseIsAttrLandmark },
    { .fn = (void *)ActorStageObjBaseIsTarget }, { .fn = (void *)ActorStageObjBaseIsEggCrystal },
    { .fn = (void *)ActorStageObjBaseIsCrystal },
    { .fn = (void *)ActorStageObjBaseSlot16ReturnFalse },
    { .fn = (void *)ActorStageObjLandmarkIsLandmark },
    { .fn = (void *)ActorStageObjLandmarkHidesHpGauge },
    { .fn = (void *)ActorStageObjBaseReturnFalse },
};
