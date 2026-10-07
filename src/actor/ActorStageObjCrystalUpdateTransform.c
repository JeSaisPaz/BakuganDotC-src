// bdc 0x088b4780 ActorStageObjCrystalUpdateTransform
#include "bdc.h"

/* Rebuilds the model matrix of the crystal stage object from its heading `rot[1]` (`wrap(pi/2 - heading)` into
   (-pi, pi]), scale and position: the Y rotation times the scale is written to the model's
   `rootMatrix`, then the translation `pos` is copied into its last column and its W is forced to
   1.0f. */

void ActorStageObjCrystalUpdateTransform(ActorStageObjCrystal *self)
{
  float heading;
  float c;
  float s;
  float *m;

  m = self->base.base.data->rootMatrix;
  heading = 1.5707964f - self->base.base.rot[1];
  if (!(heading <= 3.1415927f)) {
    heading = heading - 6.2831855f;
  } else if (heading <= -3.1415927f) {
    heading = heading + 6.2831855f;
  }
  /* vrot of heading * S703 (2/pi, bank constant): cos/sin of the heading. */
  c = __builtin_cosf(heading);
  s = __builtin_sinf(heading);
  /* vmmul.q E200,E300,E000 is M200 = M000 · M300 = R · diag(scale) (as for vmmul_q_transp3): column j
     of R (C000 [C,0,-S,0], C010 [0,1,0,0], C020 [S,0,C,0], C030 [0,0,0,1]) times scale[j]. */
  m[0] = c * self->base.base.scale[0];
  m[1] = 0.0f;
  m[2] = -s * self->base.base.scale[0];
  m[3] = 0.0f;
  m[4] = 0.0f;
  m[5] = self->base.base.scale[1];
  m[6] = 0.0f;
  m[7] = 0.0f;
  m[8] = s * self->base.base.scale[2];
  m[9] = 0.0f;
  m[10] = c * self->base.base.scale[2];
  m[11] = 0.0f;
  m[12] = 0.0f;
  m[13] = 0.0f;
  m[14] = 0.0f;
  m[15] = 1.0f;
  m = self->base.base.data->rootMatrix;
  m[12] = self->base.base.pos[0];
  m[13] = self->base.base.pos[1];
  m[14] = self->base.base.pos[2];
  m[15] = self->base.base.pos[3];
  self->base.base.data->rootMatrix[15] = 1.0f;
}
