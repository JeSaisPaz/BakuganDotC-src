// bdc 0x088b0f84 ActorStageObjPropApplyMotion
#include "bdc.h"

/* Slot-19 virtual of the knock-over prop (overrides `ActorStageObjBaseReturnFalse`): applies a
   frame of motion `delta` with a spring on the height offset `+0x22c` (towards 2.3), moves the
   position, and when the prop spins (`+0x210`) multiplies the model matrix by the rotation block
   `+0x170..+0x1ac` (rotation * model); returns 0 in every case. */

int ActorStageObjPropApplyMotion(ActorStageObjProp *self, float *delta)
{
  float *spring = (float *)self->base.springY;
  float *pos = self->base.base.pos;
  float (*rot)[4] = self->base.rotMatrix;
  float *m;
  float b[4][4];
  int j;

  *spring = *spring - (2.3f - *spring) * 0.2f;
  delta[1] = delta[1] + *spring;
  /* vadd.t: xyz only, w stored back unchanged. */
  pos[0] = pos[0] + delta[0];
  pos[1] = pos[1] + delta[1];
  pos[2] = pos[2] + delta[2];
  if (*(float *)self->base.spin != 0.0f) {
    m = self->base.base.data->rootMatrix;
    /* vmmul.q M000, M100 (rot), M200 (model): column j = sum over k of model[j][k] * rot column k. */
    for (j = 0; j < 4; j++) {
      b[j][0] = m[j * 4 + 0];
      b[j][1] = m[j * 4 + 1];
      b[j][2] = m[j * 4 + 2];
      b[j][3] = m[j * 4 + 3];
    }
    for (j = 0; j < 4; j++) {
      m[j * 4 + 0] = b[j][0] * rot[0][0] + b[j][1] * rot[1][0] + b[j][2] * rot[2][0] + b[j][3] * rot[3][0];
      m[j * 4 + 1] = b[j][0] * rot[0][1] + b[j][1] * rot[1][1] + b[j][2] * rot[2][1] + b[j][3] * rot[3][1];
      m[j * 4 + 2] = b[j][0] * rot[0][2] + b[j][1] * rot[1][2] + b[j][2] * rot[2][2] + b[j][3] * rot[3][2];
      m[j * 4 + 3] = b[j][0] * rot[0][3] + b[j][1] * rot[1][3] + b[j][2] * rot[2][3] + b[j][3] * rot[3][3];
    }
  }
  m = self->base.base.data->rootMatrix;
  m[12] = pos[0];
  m[13] = pos[1];
  m[14] = pos[2];
  m[15] = pos[3];
  return 0;
}
