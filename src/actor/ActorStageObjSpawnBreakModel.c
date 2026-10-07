// bdc 0x088a9d24 ActorStageObjSpawnBreakModel
#include "bdc.h"

/* Builds the collapse model of a destroyed stage object: for kinds with a break model
   (`ActorStageObjKindGetBreakPiece`, pieces 1..3 `f0_break_building01`, `f0_break_warehouse01`,
   `f0_break_crane01`) and none yet (`breakModel`), creates a 0x340-byte `ActorStageObjBreakPieceCtor`
   with the object's matrix, lowers it (kind 6: translation y - 130; others: onto the ground,
   `CollisionFindGroundPoint`), syncs its `pos` and heading (`rot[1]`) and then drops an item
   (`ActorStageObjDropItem`). Kinds 0x6a/0x6b/0x8c/0x99/0x9a without a break model only drop the
   item; a kind whose break model already exists does nothing. Called by the stage-object states 2, 4, 6. */

void ActorStageObjSpawnBreakModel(ActorStageObjBase *self)
{
  float size[4];
  float groundCopy[4];
  float ground[4];
  ActorStageObjBreakPiece *piece;
  ActorStageObjBreakPiece *mem;
  ActorStageObjBase *model;
  GmoModel *dst;
  GmoModel *src;
  bool fromLow;
  int pieceId;
  int i;

  pieceId = ActorStageObjKindGetBreakPiece(self->kind);
  if (pieceId == 0) {
    if (self->kind == 0x6a || self->kind == 0x6b || self->kind == 0x8c ||
        self->kind == 0x99 || self->kind == 0x9a) {
      ActorStageObjDropItem(self);
    }
    return;
  }
  if (self->breakModel != NULL) {
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
  self->breakModel = piece;
  dst = piece->base.base.data;
  src = self->base.data;
  for (i = 0; i < 16; i++) {
    dst->rootMatrix[i] = src->rootMatrix[i];
  }

  if (self->kind == 6) {
    model = (ActorStageObjBase *)self->breakModel;
    model->base.data->rootMatrix[13] = model->base.data->rootMatrix[13] - 130.0f;
  } else {
    CollisionFindGroundPoint(ground, &self->base.data->rootMatrix[12], 0x3fbf2100);
    for (i = 0; i < 4; i++) {
      groundCopy[i] = ground[i];
    }
    model = (ActorStageObjBase *)self->breakModel;
    model->base.data->rootMatrix[13] = groundCopy[1];
  }

  model = (ActorStageObjBase *)self->breakModel;
  for (i = 0; i < 4; i++) {
    model->base.pos[i] = model->base.data->rootMatrix[12 + i];
  }
  ((ActorStageObjBase *)self->breakModel)->base.rot[1] = self->base.rot[1];
  ActorStageObjDropItem(self);
}
