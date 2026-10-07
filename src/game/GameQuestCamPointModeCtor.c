// bdc 0x088f8010 GameQuestCamPointModeCtor
#include "bdc.h"

/* Constructor of the point camera mode of the quest-field camera system (see
   `GameQuestCamCtrlCtor`), used for type-2 camera table entries and called by
   `GameQuestCamCtrlStartPointMode`: `GameQuestCamModeBaseCtor`, installs
   `g_gameQuestCamPointVtbl`, zeroes `followOffset` and `viewDir`,
   copies the descriptor's `pos` to `descPos`, copies the followed object's point (its vtable slot 2)
   to `targetPos`, takes the descriptor's `entry`, rotates the `up` axis of
   `g_gameQuestCamPointModeAxisConsts` by the entry's `rotation` quaternion (matrix built with
   VFPU prefixes, `vtfm3.t`) into `viewDir` (w = 0), sets `followOffset = viewDir * -sideOffset`,
   then normalises `viewDir` (`up` instead when its squared length is <= 0.0001), sets `kind` 2 and
   returns `self`. `followOffset` and `viewDir` start as (0, 0, 0, 0) (VFPU bank constant C720) and
   end with w = 0. */

typedef struct CamFollowed {
  void *unk0;
  const VtblEntry *vtbl; /* +0x04 */
} CamFollowed;

GameQuestCamPointMode *GameQuestCamPointModeCtor(GameQuestCamPointMode *self, void *desc)
{
  GameQuestCamPointModeDesc *d = (GameQuestCamPointModeDesc *)desc;
  CamFollowed *followed;
  const VtblEntry *e;
  const ScePspFVector4 *point;
  const float *q;
  const ScePspFVector4 *up;
  float a[3][4]; /* rows of the first prefixed quaternion matrix (M100) */
  float b[3][4]; /* columns of the second prefixed quaternion matrix (M200) */
  float m[3][3]; /* 3x3 part of M100 * M200 (vmmul.q E000, E200, E100): rotation matrix */
  float scale;
  float len2;
  float k;
  int i;
  int j;

  GameQuestCamModeBaseCtor(&self->base, desc);
  self->base.base.base.vtbl = g_gameQuestCamPointVtbl;
  /* Bank constant C720 = (0, 0, 0, 0). */
  self->followOffset.x = 0.0f;
  self->followOffset.y = 0.0f;
  self->followOffset.z = 0.0f;
  self->followOffset.w = 0.0f;
  self->viewDir.x = 0.0f;
  self->viewDir.y = 0.0f;
  self->viewDir.z = 0.0f;
  self->viewDir.w = 0.0f;
  self->descPos = d->pos;
  followed = (CamFollowed *)self->base.base.base.followed;
  e = &followed->vtbl[2];
  point = ((const ScePspFVector4 *(*)(void *))e->fn)((u8 *)followed + e->delta);
  self->base.targetPos = *point;
  self->entry = d->entry;

  /* Quaternion (x, y, z, w) -> rotation matrix as the product of two sign/swizzle matrices. */
  q = self->entry->rotation;
  a[0][0] = q[3];  a[0][1] = -q[2]; a[0][2] = q[1];  a[0][3] = q[0];
  a[1][0] = q[2];  a[1][1] = q[3];  a[1][2] = -q[0]; a[1][3] = q[1];
  a[2][0] = -q[1]; a[2][1] = q[0];  a[2][2] = q[3];  a[2][3] = q[2];
  b[0][0] = q[3];  b[0][1] = q[2];  b[0][2] = -q[1]; b[0][3] = q[0];
  b[1][0] = -q[2]; b[1][1] = q[3];  b[1][2] = q[0];  b[1][3] = q[1];
  b[2][0] = q[1];  b[2][1] = -q[0]; b[2][2] = q[3];  b[2][3] = q[2];
  for (i = 0; i < 3; i++) {
    for (j = 0; j < 3; j++) {
      m[i][j] = a[i][0] * b[j][0] + a[i][1] * b[j][1] + a[i][2] * b[j][2] + a[i][3] * b[j][3];
    }
  }
  /* viewDir = m * up (w = 0, row 3 of the matrix was reset to identity). */
  up = &g_gameQuestCamPointModeAxisConsts.up;
  self->viewDir.x = m[0][0] * up->x + m[0][1] * up->y + m[0][2] * up->z;
  self->viewDir.y = m[1][0] * up->x + m[1][1] * up->y + m[1][2] * up->z;
  self->viewDir.z = m[2][0] * up->x + m[2][1] * up->y + m[2][2] * up->z;
  self->viewDir.w = 0.0f;

  self->followOffset = self->viewDir;
  scale = -self->entry->sideOffset;
  self->followOffset.x = self->followOffset.x * scale;
  self->followOffset.y = self->followOffset.y * scale;
  self->followOffset.z = self->followOffset.z * scale;
  self->followOffset.w = 0.0f; /* lane 3 of C710 = bank constant S713 */
  len2 = self->viewDir.x * self->viewDir.x + self->viewDir.y * self->viewDir.y +
         self->viewDir.z * self->viewDir.z;
  if (!(len2 <= 0.0001f)) {
    len2 = self->viewDir.x * self->viewDir.x + self->viewDir.y * self->viewDir.y +
           self->viewDir.z * self->viewDir.z;
    k = (len2 == 0.0f) ? 0.0f : VfRsq(len2);
    self->viewDir.x = VfSat1(self->viewDir.x * k);
    self->viewDir.y = VfSat1(self->viewDir.y * k);
    self->viewDir.z = VfSat1(self->viewDir.z * k);
    self->viewDir.w = 0.0f; /* masked lane of C710 = bank constant S713 */
  } else {
    self->viewDir = g_gameQuestCamPointModeAxisConsts.up;
  }
  self->base.kind = 2;
  return self;
}
