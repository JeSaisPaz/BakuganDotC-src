// bdc 0x088ac5c8 ActorStageObjRebuildMatrix
#include "bdc.h"

/* Rebuilds the model matrix from heading `+0x34`, scale `+0x40..+0x48` and position when `force` is
   set or the dirty flag `+0x287` is set (same matrix as `ActorStageObjUpdateTransform`). Called
   by `ActorStageObjUpdate`, `ActorStageObjMineCtor` and `ActorStageObjBreakPieceUpdate`. */

void ActorStageObjRebuildMatrix(ActorStageObjBase *self, char force)
{
  float *m;
  float sx;
  float sy;
  float sz;
  float c;
  float s;

  if (force == '\0' && self->matrixDirty == '\0') {
    return;
  }
  m = self->base.data->rootMatrix;
  sx = self->base.scale[0];
  sy = self->base.scale[1];
  sz = self->base.scale[2];
  /* vrot over heading * S703 (2/pi): cos/sin of the heading in radians. */
  c = VfCosQuarter(VfMul(self->base.rot[1], 0.636619747f));
  s = VfSinQuarter(VfMul(self->base.rot[1], 0.636619747f));
  /* vmmul.q E200, E300, E000: Y rotation [C,0,-S,0] / (0,1,0,0) / [S,0,C,0] / (0,0,0,1) times the
     diagonal scale matrix, column j = scale[j] * rotation column j. */
  m[0] = VfDot2(sx, 0.0f, c, s);
  m[1] = VfDot2(sx, 0.0f, 0.0f, 0.0f);
  m[2] = VfDot2(sx, 0.0f, -s, c);
  m[3] = VfDot2(sx, 0.0f, 0.0f, 0.0f);
  m[4] = VfDot3(0.0f, sy, 0.0f, c, 0.0f, s);
  m[5] = VfDot2(sy, 0.0f, 1.0f, 0.0f);
  m[6] = VfDot3(0.0f, sy, 0.0f, -s, 0.0f, c);
  m[7] = VfDot2(sy, 0.0f, 0.0f, 0.0f);
  m[8] = VfDot2(0.0f, sz, c, s);
  m[9] = VfDot2(sz, 0.0f, 0.0f, 0.0f);
  m[10] = VfDot2(0.0f, sz, -s, c);
  m[11] = VfDot2(sz, 0.0f, 0.0f, 0.0f);
  /* Column 3 ((0,0,0,1) from the product) is overwritten by the full position vec4. */
  m = self->base.data->rootMatrix;
  m[12] = self->base.pos[0];
  m[13] = self->base.pos[1];
  m[14] = self->base.pos[2];
  m[15] = self->base.pos[3];
}
