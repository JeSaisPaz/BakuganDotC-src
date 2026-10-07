// bdc 0x08af29a4 g_actorStageObjBreakPieceVtbl
#include "bdc.h"

__typeof__(VtblEntry[20]) g_actorStageObjBreakPieceVtbl = {
    {0}, { .fn = (void *)ActorStageObjBreakPieceDtor }, { .fn = (void *)GfxModelInitFields },
    { .fn = (void *)GfxModelSetFogEnabled }, { .fn = (void *)GfxModelSetFog },
    { .fn = (void *)GfxModelDistanceFade }, { .fn = (void *)GfxModelSetMotionSpeed },
    { .fn = (void *)ActorStageObjBreakPieceUpdate }, { .fn = (void *)ActorStageObjBreakPieceDraw },
    { .fn = (void *)GfxModelSetToon }, { .fn = (void *)ActorStageObjBaseSetupMaterials },
    { .fn = (void *)ActorStageObjBaseBreak }, { .fn = (void *)ActorStageObjBaseIsAttrLandmark },
    { .fn = (void *)ActorStageObjBaseIsTarget }, { .fn = (void *)ActorStageObjBaseIsEggCrystal },
    { .fn = (void *)ActorStageObjBaseIsCrystal },
    { .fn = (void *)ActorStageObjBaseSlot16ReturnFalse },
    { .fn = (void *)ActorStageObjBaseIsLandmark }, { .fn = (void *)ActorStageObjBaseHidesHpGauge },
    { .fn = (void *)ActorStageObjBaseReturnFalse },
};
