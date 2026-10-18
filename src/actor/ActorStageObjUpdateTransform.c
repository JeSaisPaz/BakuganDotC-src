// bdc 0x088a24cc ActorStageObjUpdateTransform
#include "bdc.h"

/* Rebuilds the model matrix of a stage object from its heading `rot[1]` (`wrap(pi/2 - heading)` into
   (-pi, pi]), scale and position: the Y rotation times the scale is written to the model's
   `rootMatrix`, then the translation `pos` is copied into its last column and its W is forced to
   1.0f. */

void ActorStageObjUpdateTransform(ActorStageObjBase *self)
{
  float heading;
  float c;
  float s;
  float *m;

  m = self->base.data->rootMatrix;
  heading = 1.5707964f - self->base.rot[1];
  if (!(heading <= 3.1415927f)) {
    heading = heading - 6.2831855f;
  } else if (heading <= -3.1415927f) {
    heading = heading + 6.2831855f;
  }
  /* vrot of heading * S703 (2/pi, bank constant): cos/sin of the heading. */
  c = VfCosQuarter(VfMul(heading, 0.636619747f));
  s = VfSinQuarter(VfMul(heading, 0.636619747f));
  /* vmmul.q E200,E300,E000 is M200 = M000 · M300 = R · diag(scale) (as for vmmul_q_transp3): column j
     of R (C000 [C,0,-S,0], C010 [0,1,0,0], C020 [S,0,C,0], C030 [0,0,0,1]) times scale[j]. */
  m[0] = VfDot2(self->base.scale[0], 0.0f, c, s);
  m[1] = VfDot2(self->base.scale[0], 0.0f, 0.0f, 0.0f);
  m[2] = VfDot2(self->base.scale[0], 0.0f, -s, c);
  m[3] = VfDot2(self->base.scale[0], 0.0f, 0.0f, 0.0f);
  m[4] = VfDot3(0.0f, self->base.scale[1], 0.0f, c, 0.0f, s);
  m[5] = VfDot2(self->base.scale[1], 0.0f, 1.0f, 0.0f);
  m[6] = VfDot3(0.0f, self->base.scale[1], 0.0f, -s, 0.0f, c);
  m[7] = VfDot2(self->base.scale[1], 0.0f, 0.0f, 0.0f);
  m[8] = VfDot2(0.0f, self->base.scale[2], c, s);
  m[9] = VfDot2(self->base.scale[2], 0.0f, 0.0f, 0.0f);
  m[10] = VfDot2(0.0f, self->base.scale[2], -s, c);
  m[11] = VfDot2(self->base.scale[2], 0.0f, 0.0f, 0.0f);
  m[12] = 0.0f;
  m[13] = 0.0f;
  m[14] = 0.0f;
  m[15] = 1.0f;
  m = self->base.data->rootMatrix;
  m[12] = self->base.pos[0];
  m[13] = self->base.pos[1];
  m[14] = self->base.pos[2];
  m[15] = self->base.pos[3];
  self->base.data->rootMatrix[15] = 1.0f;
}
