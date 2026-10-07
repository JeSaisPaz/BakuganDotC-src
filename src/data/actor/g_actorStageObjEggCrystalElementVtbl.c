// bdc 0x08af25c4 g_actorStageObjEggCrystalElementVtbl
#include "bdc.h"

__typeof__(VtblEntry[20]) g_actorStageObjEggCrystalElementVtbl = {
    {0}, { .fn = (void *)ActorStageObjEggCrystalElementDtor }, { .fn = (void *)GfxModelInitFields },
    { .fn = (void *)GfxModelSetFogEnabled }, { .fn = (void *)GfxModelSetFog },
    { .fn = (void *)GfxModelDistanceFade }, { .fn = (void *)GfxModelSetMotionSpeed },
    { .fn = (void *)ActorStageObjEggCrystalElementUpdate },
    { .fn = (void *)ActorStageObjEggCrystalDraw }, { .fn = (void *)GfxModelSetToon },
    { .fn = (void *)ActorStageObjBaseSetupMaterials },
    { .fn = (void *)ActorStageObjEggCrystalBreak },
    { .fn = (void *)ActorStageObjBaseIsAttrLandmark }, { .fn = (void *)ActorStageObjBaseIsTarget },
    { .fn = (void *)ActorStageObjEggCrystalIsEggCrystal },
    { .fn = (void *)ActorStageObjBaseIsCrystal },
    { .fn = (void *)ActorStageObjBaseSlot16ReturnFalse },
    { .fn = (void *)ActorStageObjBaseIsLandmark }, { .fn = (void *)ActorStageObjBaseHidesHpGauge },
    { .fn = (void *)ActorStageObjBaseReturnFalse },
};
