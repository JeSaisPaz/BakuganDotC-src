// bdc 0x08af2524 g_actorStageObjEggCrystalVtbl
#include "bdc.h"

__typeof__(VtblEntry[20]) g_actorStageObjEggCrystalVtbl = {
    {0}, { .fn = (void *)ActorStageObjEggCrystalDtor }, { .fn = (void *)GfxModelInitFields },
    { .fn = (void *)GfxModelSetFogEnabled }, { .fn = (void *)GfxModelSetFog },
    { .fn = (void *)GfxModelDistanceFade }, { .fn = (void *)GfxModelSetMotionSpeed },
    { .fn = (void *)ActorStageObjEggCrystalUpdate }, { .fn = (void *)ActorStageObjEggCrystalDraw },
    { .fn = (void *)GfxModelSetToon }, { .fn = (void *)ActorStageObjBaseSetupMaterials },
    { .fn = (void *)ActorStageObjEggCrystalBreak },
    { .fn = (void *)ActorStageObjBaseIsAttrLandmark }, { .fn = (void *)ActorStageObjBaseIsTarget },
    { .fn = (void *)ActorStageObjEggCrystalIsEggCrystal },
    { .fn = (void *)ActorStageObjBaseIsCrystal },
    { .fn = (void *)ActorStageObjBaseSlot16ReturnFalse },
    { .fn = (void *)ActorStageObjBaseIsLandmark }, { .fn = (void *)ActorStageObjBaseHidesHpGauge },
    { .fn = (void *)ActorStageObjBaseReturnFalse },
};
