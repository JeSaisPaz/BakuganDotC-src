// bdc 0x088afbe0 ActorStageObjBreakPieceRemove
#include "bdc.h"

/* Piece handler 0 (table `0x08a84c3c`): schedules the piece for deletion
   (`CoreObjectDeferDelete`). */

void ActorStageObjBreakPieceRemove(ActorStageObjBreakPiece *self)

{
  CoreObjectDeferDelete((CoreObject *)self,0);
  return;
}

