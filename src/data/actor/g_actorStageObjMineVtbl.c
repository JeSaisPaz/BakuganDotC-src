// bdc 0x08af2674 g_actorStageObjMineVtbl
#include "bdc.h"

__typeof__(VtblEntry[20]) g_actorStageObjMineVtbl = {
    {0}, { .fn = (void *)ActorStageObjMineDtor }, { .fn = (void *)GfxModelInitFields },
    { .fn = (void *)GfxModelSetFogEnabled }, { .fn = (void *)GfxModelSetFog },
    { .fn = (void *)GfxModelDistanceFade }, { .fn = (void *)GfxModelSetMotionSpeed },
    { .fn = (void *)ActorStageObjMineUpdate }, { .fn = (void *)ActorStageObjMineDraw },
    { .fn = (void *)GfxModelSetToon }, { .fn = (void *)ActorStageObjMineSetupMaterials },
    { .fn = (void *)ActorStageObjMineBreak }, { .fn = (void *)ActorStageObjBaseIsAttrLandmark },
    { .fn = (void *)ActorStageObjBaseIsTarget }, { .fn = (void *)ActorStageObjBaseIsEggCrystal },
    { .fn = (void *)ActorStageObjBaseIsCrystal },
    { .fn = (void *)ActorStageObjMineSlot16ReturnTrue },
    { .fn = (void *)ActorStageObjBaseIsLandmark }, { .fn = (void *)ActorStageObjBaseHidesHpGauge },
    { .fn = (void *)ActorStageObjBaseReturnFalse },
};
