// bdc 0x08af2ae4 g_actorStageObjPropVtbl
#include "bdc.h"

__typeof__(VtblEntry[20]) g_actorStageObjPropVtbl = {
    {0}, { .fn = (void *)ActorStageObjPropDtor }, { .fn = (void *)GfxModelInitFields },
    { .fn = (void *)GfxModelSetFogEnabled }, { .fn = (void *)GfxModelSetFog },
    { .fn = (void *)GfxModelDistanceFade }, { .fn = (void *)GfxModelSetMotionSpeed },
    { .fn = (void *)ActorStageObjPropUpdate }, { .fn = (void *)ActorStageObjPropDraw },
    { .fn = (void *)GfxModelSetToon }, { .fn = (void *)ActorStageObjBaseSetupMaterials },
    { .fn = (void *)ActorStageObjPropBreak }, { .fn = (void *)ActorStageObjBaseIsAttrLandmark },
    { .fn = (void *)ActorStageObjBaseIsTarget }, { .fn = (void *)ActorStageObjBaseIsEggCrystal },
    { .fn = (void *)ActorStageObjBaseIsCrystal },
    { .fn = (void *)ActorStageObjBaseSlot16ReturnFalse },
    { .fn = (void *)ActorStageObjBaseIsLandmark }, { .fn = (void *)ActorStageObjBaseHidesHpGauge },
    { .fn = (void *)ActorStageObjPropApplyMotion },
};
