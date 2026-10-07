// bdc 0x088afde0 ActorStageObjBreakPieceCtor
#include "bdc.h"

/* Constructor of a debris piece left by a destroyed stage object: `ActorStageObjBaseCtorByName`
   with model `g_actorStageObjBreakPieceModelNames[piece]` at `(y, y, y)`, installs
   `g_actorStageObjBreakPieceVtbl` and stores `piece` and the source kind `type`. From the model
   bounds (`ActorStageObjGetBounds`, Y components zeroed) it sets `diagonal2` = |min - max| (XZ)
   and `extents` = max - min when min.x < 0, else max + min (xyz; w from max). The scale is the
   source object's `size` relative to the piece: X = size.x / extents.x * 0.9 and Z likewise
   (clamped to 0.4..1.2), Y = |size| / diagonal2 * 0.8 (clamped to 0.4..1.1); kind 6 forces
   (1, 1, 1), kind 0x7f sets X to 0.5. The scale goes to `scale` and the model root matrix
   diagonal. Resets the fade (`fade` = `baseAlpha` = 1, `fadeState` = `hidden` = 0), sets the
   ambient colour (alpha 0 for piece 0, else lit with 0.3 grey, alpha 1), `step` = 0, applies the
   arena light colours (`BtlStageApplyLightColors`) and returns `self`. The original reads
   `size` with `lv.q` (16-byte aligned). */

void *ActorStageObjBreakPieceCtor(ActorStageObjBreakPiece *self, int piece, int type, const float *size, int y)
{
  float pos[4];
  float maxB[4];
  float minB[4];
  float scale[4];
  float diff[4];
  float one[4];
  float len;
  float *bounds;
  float sx;
  float sy;
  float sz;

  pos[2] = (float)y;
  pos[1] = pos[2];
  pos[0] = pos[2];
  pos[3] = 0.0f;
  ActorStageObjBaseCtorByName(&self->base, g_actorStageObjBreakPieceModelNames[piece], pos);
  self->base.base.base.vtable = g_actorStageObjBreakPieceVtbl;
  self->piece = piece;
  self->sourceKind = type;
  bounds = ActorStageObjGetBounds(&self->base);
  maxB[0] = bounds[4];
  maxB[1] = bounds[5];
  maxB[2] = bounds[6];
  maxB[3] = bounds[7];
  bounds = ActorStageObjGetBounds(&self->base);
  minB[0] = bounds[0];
  minB[1] = bounds[1];
  minB[2] = bounds[2];
  minB[3] = bounds[3];
  maxB[1] = 0.0f;
  minB[1] = 0.0f;
  /* diagonal2 = |min - max| (xyz). */
  diff[0] = minB[0] - maxB[0];
  diff[1] = minB[1] - maxB[1];
  diff[2] = minB[2] - maxB[2];
  self->base.diagonal2 = __builtin_sqrtf(diff[0] * diff[0] + diff[1] * diff[1] + diff[2] * diff[2]);
  /* The asm first zeroes `extents` (bank C720); both branches overwrite all four lanes. */
  if (minB[0] < 0.0f) {
    self->base.extents[0] = maxB[0] - minB[0];
    self->base.extents[1] = maxB[1] - minB[1];
    self->base.extents[2] = maxB[2] - minB[2];
  } else {
    self->base.extents[0] = maxB[0] + minB[0];
    self->base.extents[1] = maxB[1] + minB[1];
    self->base.extents[2] = maxB[2] + minB[2];
  }
  self->base.extents[3] = maxB[3];
  scale[2] = 0.0f;
  scale[1] = 0.0f;
  scale[0] = 0.0f;
  scale[3] = 0.0f;
  len = __builtin_sqrtf(size[0] * size[0] + size[1] * size[1] + size[2] * size[2]);
  sy = len / self->base.diagonal2 * 0.800000012f;
  sx = size[0] / self->base.extents[0] * 0.899999976f;
  scale[0] = sx;
  sz = size[2] / self->base.extents[2] * 0.899999976f;
  scale[2] = sz;
  if (sx < 0.400000006f) {
    sx = 0.400000006f;
  } else if (!(sx <= 1.20000005f)) {
    sx = 1.20000005f;
  }
  scale[0] = sx;
  if (sy < 0.400000006f) {
    sy = 0.400000006f;
  } else if (!(sy <= 1.10000002f)) {
    sy = 1.10000002f;
  }
  scale[1] = sy;
  sz = scale[2];
  if (sz < 0.400000006f) {
    sz = 0.400000006f;
  } else if (!(sz <= 1.20000005f)) {
    sz = 1.20000005f;
  }
  scale[2] = sz;
  if (self->sourceKind < 7) {
    if (self->sourceKind >= 6) {
      one[2] = 1.0f;
      one[1] = 1.0f;
      one[0] = 1.0f;
      one[3] = 0.0f;
      scale[0] = one[0];
      scale[1] = one[1];
      scale[2] = one[2];
      scale[3] = one[3];
    }
  } else if (self->sourceKind == 0x7f) {
    scale[0] = 0.5f;
  }
  self->base.base.scale[0] = scale[0];
  self->base.base.scale[1] = scale[1];
  self->base.base.scale[2] = scale[2];
  self->base.base.scale[3] = scale[3];
  self->base.base.data->rootMatrix[0] = self->base.base.scale[0];
  self->base.base.data->rootMatrix[5] = self->base.base.scale[1];
  self->base.base.data->rootMatrix[10] = self->base.base.scale[2];
  self->fade = 1.0f;
  self->fadeState = 0;
  self->hidden = 0;
  self->baseAlpha = 1.0f;
  if (self->piece == 0) {
    self->base.base.ambient[3] = 0.0f;
  } else {
    self->base.base.lighting = 1;
    self->base.base.ambient[0] = 0.300000012f;
    self->base.base.ambient[1] = 0.300000012f;
    self->base.base.ambient[2] = 0.300000012f;
    self->base.base.ambient[3] = 1.0f;
  }
  self->step = 0;
  BtlStageApplyLightColors(self);
  return self;
}
