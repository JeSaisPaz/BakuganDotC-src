// bdc 0x08af2714 g_actorStageObjWindGeneratorVtbl
#include "bdc.h"

__typeof__(VtblEntry[20]) g_actorStageObjWindGeneratorVtbl = {
    {0}, { .fn = (void *)ActorStageObjWindGeneratorDtor }, { .fn = (void *)GfxModelInitFields },
    { .fn = (void *)GfxModelSetFogEnabled }, { .fn = (void *)GfxModelSetFog },
    { .fn = (void *)GfxModelDistanceFade }, { .fn = (void *)GfxModelSetMotionSpeed },
    { .fn = (void *)ActorStageObjWindGeneratorUpdate },
    { .fn = (void *)ActorStageObjWindGeneratorDraw }, { .fn = (void *)GfxModelSetToon },
    { .fn = (void *)ActorStageObjBaseSetupMaterials }, { .fn = (void *)ActorStageObjBaseBreak },
    { .fn = (void *)ActorStageObjBaseIsAttrLandmark }, { .fn = (void *)ActorStageObjBaseIsTarget },
    { .fn = (void *)ActorStageObjBaseIsEggCrystal }, { .fn = (void *)ActorStageObjBaseIsCrystal },
    { .fn = (void *)ActorStageObjWindGeneratorSlot16ReturnTrue },
    { .fn = (void *)ActorStageObjBaseIsLandmark }, { .fn = (void *)ActorStageObjBaseHidesHpGauge },
    { .fn = (void *)ActorStageObjBaseReturnFalse },
};
