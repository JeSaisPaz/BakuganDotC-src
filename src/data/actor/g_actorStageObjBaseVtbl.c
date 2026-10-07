// bdc 0x08af2904 g_actorStageObjBaseVtbl
#include "bdc.h"

__typeof__(VtblEntry[20]) g_actorStageObjBaseVtbl = {
    {0}, { .fn = (void *)ActorStageObjBaseDtor }, { .fn = (void *)GfxModelInitFields },
    { .fn = (void *)GfxModelSetFogEnabled }, { .fn = (void *)GfxModelSetFog },
    { .fn = (void *)GfxModelDistanceFade }, { .fn = (void *)GfxModelSetMotionSpeed },
    { .fn = (void *)ActorStageObjUpdate }, { .fn = (void *)ActorStageObjBaseDraw },
    { .fn = (void *)GfxModelSetToon }, { .fn = (void *)ActorStageObjBaseSetupMaterials },
    { .fn = (void *)ActorStageObjBaseBreak }, { .fn = (void *)ActorStageObjBaseIsAttrLandmark },
    { .fn = (void *)ActorStageObjBaseIsTarget }, { .fn = (void *)ActorStageObjBaseIsEggCrystal },
    { .fn = (void *)ActorStageObjBaseIsCrystal },
    { .fn = (void *)ActorStageObjBaseSlot16ReturnFalse },
    { .fn = (void *)ActorStageObjBaseIsLandmark }, { .fn = (void *)ActorStageObjBaseHidesHpGauge },
    { .fn = (void *)ActorStageObjBaseReturnFalse },
};
