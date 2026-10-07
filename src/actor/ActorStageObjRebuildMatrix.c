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
  c = __builtin_cosf(self->base.rot[1]);
  s = __builtin_sinf(self->base.rot[1]);
  /* vmmul.q E200, E300, E000: Y rotation [C,0,-S,0] / (0,1,0,0) / [S,0,C,0] / (0,0,0,1) times the
     diagonal scale matrix, column j = scale[j] * rotation column j. */
  m[0] = sx * c;
  m[1] = 0.0f;
  m[2] = sx * -s;
  m[3] = 0.0f;
  m[4] = 0.0f;
  m[5] = sy;
  m[6] = 0.0f;
  m[7] = 0.0f;
  m[8] = sz * s;
  m[9] = 0.0f;
  m[10] = sz * c;
  m[11] = 0.0f;
  /* Column 3 ((0,0,0,1) from the product) is overwritten by the full position vec4. */
  m = self->base.data->rootMatrix;
  m[12] = self->base.pos[0];
  m[13] = self->base.pos[1];
  m[14] = self->base.pos[2];
  m[15] = self->base.pos[3];
}
