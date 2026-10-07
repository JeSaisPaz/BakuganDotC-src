// bdc 0x088a9628 ActorStageObjSpawnRemains
#include "bdc.h"

/* Leaves rubble when a stage object is destroyed: when its kind has a remains model
   (`ActorStageObjKindGetRemainsPiece`, pieces 4..6 `f0_remainparts_size01..03`) and none was
   built yet (`remains`), allocates a 0x340-byte `ActorStageObjBreakPieceCtor` sized to the object
   (`extents`), copies the model matrix, drops it to the ground (`CollisionFindGroundPoint`),
   syncs its `pos` with the matrix translation and gives it the same heading (`rot[1]`).
   Called by the stage-object states 2, 4, 5, 6. */

void ActorStageObjSpawnRemains(ActorStageObjBase *self)
{
  float size[4];
  float ground[4];
  float groundCopy[4];
  ActorStageObjBreakPiece *piece;
  ActorStageObjBreakPiece *mem;
  ActorStageObjBase *remains;
  GmoModel *dst;
  GmoModel *src;
  bool fromLow;
  int pieceId;
  int i;

  pieceId = ActorStageObjKindGetRemainsPiece(self->kind);
  if (pieceId == 0 || self->remains != NULL) {
    return;
  }
  piece = NULL;
  MemLock();
  fromLow = MemIsAllocFromLow();
  MemSetAllocFromLow(true);
  mem = (ActorStageObjBreakPiece *)MemAlloc(sizeof(ActorStageObjBreakPiece), NULL, 0);
  MemSetAllocFromLow(fromLow);
  MemUnlock();
  if (mem != NULL) {
    for (i = 0; i < 4; i++) {
      size[i] = self->extents[i];
    }
    ActorStageObjBreakPieceCtor(mem, pieceId, self->kind, size, 0);
    piece = mem;
  }
  /* no NULL check: a failed allocation dereferences NULL below, as the original does */
  self->remains = piece;
  dst = piece->base.base.data;
  src = self->base.data;
  for (i = 0; i < 16; i++) {
    dst->rootMatrix[i] = src->rootMatrix[i];
  }

  CollisionFindGroundPoint(ground, &self->base.data->rootMatrix[12], 0x3fbf2100);
  for (i = 0; i < 4; i++) {
    groundCopy[i] = ground[i];
  }
  remains = (ActorStageObjBase *)self->remains;
  remains->base.data->rootMatrix[13] = groundCopy[1];

  remains = (ActorStageObjBase *)self->remains;
  for (i = 0; i < 4; i++) {
    remains->base.pos[i] = remains->base.data->rootMatrix[12 + i];
  }
  ((ActorStageObjBase *)self->remains)->base.rot[1] = self->base.rot[1];
}
