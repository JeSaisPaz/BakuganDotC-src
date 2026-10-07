// bdc 0x088f89ac GameQuestCamEyeSpringUpdateAxes
#include "bdc.h"

/* Slot 5 of the eye spring: rebuilds the spring's goal from the path set's quaternion
   (`*(self->pathSet + 0x10)`): the quaternion's left and right product matrices are multiplied
   into a rotation matrix (last row and column forced to identity), which transforms the `up`
   constant of `g_gameQuestCamEyeSpringAxisConsts`. The result, its xyz scaled by `dist` and
   the controller's mode `goal` xyz added, becomes `goal` (`goal.w` = 0). */

typedef struct EyeSpringPathSet {
  char pad[0x10];
  ScePspFVector4 quat;
} EyeSpringPathSet;

void GameQuestCamEyeSpringUpdateAxes(GameQuestCamEyeSpring *self)

{
  const EyeSpringPathSet *pathSet = (const EyeSpringPathSet *)self->pathSet;
  float l[4][4];
  float r[4][4];
  float m[4][4];
  float up[4];
  float x, y, z, w;
  float dist;
  GameQuestCamSpring *mode;
  int i;
  int j;
  int k;

  x = pathSet->quat.x;
  y = pathSet->quat.y;
  z = pathSet->quat.z;
  w = pathSet->quat.w;
  /* columns of the left matrix (M100) */
  l[0][0] = w;  l[0][1] = z;  l[0][2] = -y; l[0][3] = -x;
  l[1][0] = -z; l[1][1] = w;  l[1][2] = x;  l[1][3] = -y;
  l[2][0] = y;  l[2][1] = -x; l[2][2] = w;  l[2][3] = -z;
  l[3][0] = x;  l[3][1] = y;  l[3][2] = z;  l[3][3] = w;
  /* columns of the right matrix (M200) */
  r[0][0] = w;  r[0][1] = z;  r[0][2] = -y; r[0][3] = x;
  r[1][0] = -z; r[1][1] = w;  r[1][2] = x;  r[1][3] = y;
  r[2][0] = y;  r[2][1] = -x; r[2][2] = w;  r[2][3] = z;
  r[3][0] = -x; r[3][1] = -y; r[3][2] = -z; r[3][3] = w;
  /* column j, lane i of the product; row 3 and column 3 are the identity's */
  for (j = 0; j < 3; j++) {
    for (i = 0; i < 3; i++) {
      m[j][i] = l[0][i] * r[0][j];
      for (k = 1; k < 4; k++) {
        m[j][i] = m[j][i] + l[k][i] * r[k][j];
      }
    }
    m[j][3] = 0.0f;
  }
  m[3][0] = 0.0f;
  m[3][1] = 0.0f;
  m[3][2] = 0.0f;
  m[3][3] = 1.0f;
  up[0] = g_gameQuestCamEyeSpringAxisConsts.up.x;
  up[1] = g_gameQuestCamEyeSpringAxisConsts.up.y;
  up[2] = g_gameQuestCamEyeSpringAxisConsts.up.z;
  up[3] = g_gameQuestCamEyeSpringAxisConsts.up.w;
  self->base.goal.x = m[0][0] * up[0] + m[1][0] * up[1] + m[2][0] * up[2] + m[3][0] * up[3];
  self->base.goal.y = m[0][1] * up[0] + m[1][1] * up[1] + m[2][1] * up[2] + m[3][1] * up[3];
  self->base.goal.z = m[0][2] * up[0] + m[1][2] * up[1] + m[2][2] * up[2] + m[3][2] * up[3];
  self->base.goal.w = m[0][3] * up[0] + m[1][3] * up[1] + m[2][3] * up[2] + m[3][3] * up[3];
  dist = self->dist;
  self->base.goal.x = self->base.goal.x * dist;
  self->base.goal.y = self->base.goal.y * dist;
  self->base.goal.z = self->base.goal.z * dist;
  self->base.goal.w = 0.0f; /* lane S713 of the vscl.t result: the bank zero */
  mode = self->base.ctrl->mode;
  self->base.goal.x = self->base.goal.x + mode->goal.x;
  self->base.goal.y = self->base.goal.y + mode->goal.y;
  self->base.goal.z = self->base.goal.z + mode->goal.z;
}
