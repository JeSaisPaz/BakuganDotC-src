// bdc 0x088b82a8 ActorBallUpdateMatrix
#include "bdc.h"

/* Builds the world matrix of an `ActorBall` (`ActorBallCtor`) unless `flag1d1` (+0x1d1) freezes
   it: wraps the Euler angles `rot[0..2]` (+0x30/+0x34/+0x38) into (−π, π], then builds the root
   matrix `data(+0x130)->rootMatrix` from diag(`scale`) (+0x40) and a Y rotation by the heading
   π/2 − `rot[1]`, an X rotation by `rot[2]` and a Z rotation by `rot[0]` (each step is the VFPU
   `vmmul.q E200, E…00, E000`: new = old · R, row-major in memory), and stores `pos` (+0x20)
   as its translation row. When `unk148` holds a matrix, the translation row is copied back into
   `pos` and the whole root matrix is then replaced by that matrix. */

#define BALL_PI 3.1415927f
#define BALL_TWO_PI 6.2831855f

static float ActorBallWrapAngle(float a)
{
  if (!(a <= BALL_PI)) {
    a = a - BALL_TWO_PI;
  }
  else if (a <= -BALL_PI) {
    a = a + BALL_TWO_PI;
  }
  return a;
}

/* dst = a · rot, row-major 4x4 (dst[r][c] = sum_k a[r][k] * rot[k][c]): the displayed
   `vmmul.q E200, E100, E000` of the asm with M100 = a (rows loaded as columns) and M000 = rot. */
static void ActorBallMul(float *dst, const float *a, const float *rot)
{
  float tmp[16];
  int c;
  int r;

  for (r = 0; r < 4; r++) {
    for (c = 0; c < 4; c++) {
      tmp[r * 4 + c] = a[r * 4 + 0] * rot[0 * 4 + c] + a[r * 4 + 1] * rot[1 * 4 + c] +
                       a[r * 4 + 2] * rot[2 * 4 + c] + a[r * 4 + 3] * rot[3 * 4 + c];
    }
  }
  for (c = 0; c < 16; c++) {
    dst[c] = tmp[c];
  }
}

void ActorBallUpdateMatrix(ActorBall *self)

{
  float *root;
  float heading;
  float angle;
  float c;
  float s;
  float diag[16];
  float rot[16];
  int i;

  if (self->flag1d1 != 0) {
    return;
  }
  self->base.rot[0] = ActorBallWrapAngle(self->base.rot[0]);
  self->base.rot[1] = ActorBallWrapAngle(self->base.rot[1]);
  self->base.rot[2] = ActorBallWrapAngle(self->base.rot[2]);
  heading = ActorBallWrapAngle(1.5707964f - self->base.rot[1]);

  /* root = diag(scale.xyz, 1) combined with the Y rotation by the heading */
  root = self->base.data->rootMatrix;
  for (i = 0; i < 16; i++) {
    diag[i] = 0.0f;
  }
  diag[0] = self->base.scale[0];
  diag[5] = self->base.scale[1];
  diag[10] = self->base.scale[2];
  diag[15] = 1.0f;
  c = __builtin_cosf(heading);
  s = __builtin_sinf(heading);
  rot[0] = c;
  rot[1] = 0.0f;
  rot[2] = -s;
  rot[3] = 0.0f;
  rot[4] = 0.0f;
  rot[5] = 1.0f;
  rot[6] = 0.0f;
  rot[7] = 0.0f;
  rot[8] = s;
  rot[9] = 0.0f;
  rot[10] = c;
  rot[11] = 0.0f;
  rot[12] = 0.0f;
  rot[13] = 0.0f;
  rot[14] = 0.0f;
  rot[15] = 1.0f;
  ActorBallMul(root, diag, rot);

  /* X rotation by rot[2] */
  root = self->base.data->rootMatrix;
  angle = self->base.rot[2];
  c = __builtin_cosf(angle);
  s = __builtin_sinf(angle);
  rot[0] = 1.0f;
  rot[1] = 0.0f;
  rot[2] = 0.0f;
  rot[3] = 0.0f;
  rot[4] = 0.0f;
  rot[5] = c;
  rot[6] = s;
  rot[7] = 0.0f;
  rot[8] = 0.0f;
  rot[9] = -s;
  rot[10] = c;
  rot[11] = 0.0f;
  rot[12] = 0.0f;
  rot[13] = 0.0f;
  rot[14] = 0.0f;
  rot[15] = 1.0f;
  ActorBallMul(root, root, rot);

  /* Z rotation by rot[0] */
  root = self->base.data->rootMatrix;
  angle = self->base.rot[0];
  c = __builtin_cosf(angle);
  s = __builtin_sinf(angle);
  rot[0] = c;
  rot[1] = s;
  rot[2] = 0.0f;
  rot[3] = 0.0f;
  rot[4] = -s;
  rot[5] = c;
  rot[6] = 0.0f;
  rot[7] = 0.0f;
  rot[8] = 0.0f;
  rot[9] = 0.0f;
  rot[10] = 1.0f;
  rot[11] = 0.0f;
  rot[12] = 0.0f;
  rot[13] = 0.0f;
  rot[14] = 0.0f;
  rot[15] = 1.0f;
  ActorBallMul(root, root, rot);

  /* translation row = pos */
  root = self->base.data->rootMatrix;
  for (i = 0; i < 4; i++) {
    root[12 + i] = self->base.pos[i];
  }
  if (self->unk148 != NULL) {
    root = self->base.data->rootMatrix;
    for (i = 0; i < 4; i++) {
      self->base.pos[i] = root[12 + i];
    }
    root = self->base.data->rootMatrix;
    for (i = 0; i < 16; i++) {
      root[i] = self->unk148[i];
    }
  }
}
