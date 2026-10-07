// bdc 0x088f9178 GameQuestCamRailSpringUpdateAxes
#include "bdc.h"

/* Slot of the rail spring (vtable `g_gameQuestCamRailSpringVtbl`, laid out as
   `GameQuestCamEyeSpring`): slerps the quaternions of the path set's two nodes by the set's
   parameter `t` (`+0x20`) into a stack vector that is never read again, then rebuilds the goal
   from the FIRST node's quaternion alone: eight swizzled copies form the quaternion's left/right
   product matrices (E100/E200), multiplied (`vmmul.q`) with the last row and column forced to
   identity (`vidt.q`); the result transforms the `up` constant
   (`g_gameQuestCamRailSpringAxisConsts` +0x40) into `goal`. `goal` xyz is then scaled by `dist`
   (its w becomes the bank zero S713, 0.0f) and the controller's `eye` xyz added. */

typedef struct RailSpringPathNode {
  char pad[0x10];
  float quat[4];
} RailSpringPathNode;

typedef struct RailSpringPathSet {
  RailSpringPathNode *from;
  RailSpringPathNode *to;
  char pad[0x18];
  float t;
} RailSpringPathSet;

void GameQuestCamRailSpringUpdateAxes(void *spring)

{
  GameQuestCamEyeSpring *self = (GameQuestCamEyeSpring *)spring;
  const RailSpringPathSet *pathSet = (const RailSpringPathSet *)self->pathSet;
  const float *quatA = pathSet->from->quat;
  const float *quatB = pathSet->to->quat;
  const float *up = &g_gameQuestCamRailSpringAxisConsts.up.x;
  float slerp[4];
  float qb[4];
  float left[4][4];  /* [column][row]: M100 */
  float right[4][4]; /* [column][row]: M200 */
  float rot[4][4];   /* [column][row]: M000 */
  float out[4];
  float t;
  float d;
  float wa;
  float wb;
  float theta;
  float sinTheta;
  float inv;
  float x, y, z, w;
  int r;
  int c;
  int k;

  slerp[3] = 0.0f;
  slerp[2] = 0.0f;
  slerp[1] = 0.0f;
  slerp[0] = 0.0f;
  t = pathSet->t;
  for (k = 0; k < 4; k++) {
    qb[k] = quatB[k];
  }
  /* Slerp of the two node quaternions; the result is never read again (as in the binary). */
  d = VfSat1(quatA[0] * qb[0] + quatA[1] * qb[1] + quatA[2] * qb[2] + quatA[3] * qb[3]);
  wa = 1.0f - t;
  wb = t;
  if (!(d >= 0.998046875f)) {
    /* theta = acos(|d|) in quarter turns */
    if (__builtin_fabsf(d) < 0.707106769f) {
      theta = 1.0f - VfAsinQuarter(__builtin_fabsf(d));
    } else {
      theta = VfAsinQuarter(__builtin_sqrtf(1.0f - d * d));
    }
    wa = wa * theta;
    wb = wb * theta;
    wa = VfSinQuarter(wa);
    wb = VfSinQuarter(wb);
    sinTheta = VfSinQuarter(theta);
    if (d < 0.0f) {
      for (k = 0; k < 4; k++) {
        qb[k] = -qb[k];
      }
    }
    inv = VfRcp(sinTheta);
    wa = wa * inv;
    wb = wb * inv;
    if (sinTheta < 5e-05f) {
      for (k = 0; k < 4; k++) {
        slerp[k] = quatA[k];
      }
      goto slerpDone;
    }
  }
  for (k = 0; k < 4; k++) {
    slerp[k] = quatA[k] * wa + qb[k] * wb;
  }
slerpDone:
  (void)slerp;

  /* Rotation matrix of the first node's quaternion: left/right product matrices multiplied
     (vmmul.q E000, E200, E100: rot = left * right^T), last row and column identity. */
  x = quatA[0];
  y = quatA[1];
  z = quatA[2];
  w = quatA[3];
  left[0][0] = w;
  left[0][1] = z;
  left[0][2] = -y;
  left[0][3] = -x;
  left[1][0] = -z;
  left[1][1] = w;
  left[1][2] = x;
  left[1][3] = -y;
  left[2][0] = y;
  left[2][1] = -x;
  left[2][2] = w;
  left[2][3] = -z;
  left[3][0] = x;
  left[3][1] = y;
  left[3][2] = z;
  left[3][3] = w;
  right[0][0] = w;
  right[0][1] = z;
  right[0][2] = -y;
  right[0][3] = x;
  right[1][0] = -z;
  right[1][1] = w;
  right[1][2] = x;
  right[1][3] = y;
  right[2][0] = y;
  right[2][1] = -x;
  right[2][2] = w;
  right[2][3] = z;
  right[3][0] = -x;
  right[3][1] = -y;
  right[3][2] = -z;
  right[3][3] = w;
  for (c = 0; c < 4; c++) {
    for (r = 0; r < 4; r++) {
      rot[c][r] = left[0][r] * right[0][c] + left[1][r] * right[1][c] +
                  left[2][r] * right[2][c] + left[3][r] * right[3][c];
    }
  }
  /* vidt.q R003 / vidt.q C030 */
  rot[0][3] = 0.0f;
  rot[1][3] = 0.0f;
  rot[2][3] = 0.0f;
  rot[3][3] = 1.0f;
  rot[3][0] = 0.0f;
  rot[3][1] = 0.0f;
  rot[3][2] = 0.0f;
  /* vtfm4.q C000, E100, C200: out = sum over columns of rot[c] * up[c] */
  for (r = 0; r < 4; r++) {
    out[r] = rot[0][r] * up[0] + rot[1][r] * up[1] + rot[2][r] * up[2] + rot[3][r] * up[3];
  }
  (self->base).goal.x = out[0];
  (self->base).goal.y = out[1];
  (self->base).goal.z = out[2];
  (self->base).goal.w = out[3];
  /* vscl.t with dist; the stored w lane is the bank zero S713 */
  (self->base).goal.x = (self->base).goal.x * self->dist;
  (self->base).goal.y = (self->base).goal.y * self->dist;
  (self->base).goal.z = (self->base).goal.z * self->dist;
  (self->base).goal.w = 0.0f;
  (self->base).goal.x = (self->base).goal.x + ((self->base).ctrl)->eye.x;
  (self->base).goal.y = (self->base).goal.y + ((self->base).ctrl)->eye.y;
  (self->base).goal.z = (self->base).goal.z + ((self->base).ctrl)->eye.z;
}
