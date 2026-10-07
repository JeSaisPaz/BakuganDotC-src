// bdc 0x08a84c3c g_actorStageObjBreakPieceStateTable
#include "bdc.h"

__typeof__(VtblEntry[7]) g_actorStageObjBreakPieceStateTable = {
    { .fn = (void *)ActorStageObjBreakPieceRemove },
    { .fn = (void *)ActorStageObjBreakPieceCollapse01 },
    { .fn = (void *)ActorStageObjBreakPieceCollapse02 },
    { .fn = (void *)ActorStageObjBreakPieceCraneCollapse },
    { .fn = (void *)ActorStageObjBreakPieceRubbleIdle },
    { .fn = (void *)ActorStageObjBreakPieceRubbleIdle },
    { .fn = (void *)ActorStageObjBreakPieceRubbleIdle },
};
