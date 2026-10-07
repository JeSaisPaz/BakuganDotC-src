// bdc 0x08af2b94 g_actorStageObjCrystalVtbl
#include "bdc.h"

__typeof__(VtblEntry[20]) g_actorStageObjCrystalVtbl = {
    {0}, { .fn = (void *)ActorStageObjCrystalDtor }, { .fn = (void *)GfxModelInitFields },
    { .fn = (void *)GfxModelSetFogEnabled }, { .fn = (void *)GfxModelSetFog },
    { .fn = (void *)GfxModelDistanceFade }, { .fn = (void *)GfxModelSetMotionSpeed },
    { .fn = (void *)ActorStageObjCrystalUpdate }, { .fn = (void *)ActorStageObjCrystalDraw },
    { .fn = (void *)GfxModelSetToon }, { .fn = (void *)ActorStageObjCrystalSetupMaterials },
    { .fn = (void *)ActorStageObjCrystalBreak }, { .fn = (void *)ActorStageObjBaseIsAttrLandmark },
    { .fn = (void *)ActorStageObjBaseIsTarget }, { .fn = (void *)ActorStageObjBaseIsEggCrystal },
    { .fn = (void *)ActorStageObjCrystalIsCrystal },
    { .fn = (void *)ActorStageObjBaseSlot16ReturnFalse },
    { .fn = (void *)ActorStageObjBaseIsLandmark }, { .fn = (void *)ActorStageObjBaseHidesHpGauge },
    { .fn = (void *)ActorStageObjBaseReturnFalse },
};
