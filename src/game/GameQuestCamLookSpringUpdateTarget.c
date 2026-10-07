// bdc 0x088f8d38 GameQuestCamLookSpringUpdateTarget
#include "bdc.h"

/* Slot 4 of the look-at spring: copies the owner's position (`GameQuestCamSpringGetOwnerPos`)
   into `goal`, then takes the camera's horizontal view direction (`ctrl->lookAt - ctrl->eye`, y
   forced to 0). When both |x| and |z| are <= 0.0001 it returns with `goal` = owner position.
   Otherwise the direction is normalised (saturated to [-1,1]; a zero vector gets scale 0), an
   offset `dir * lookParams.z + cross(dir, up) * lookParams.x` (up =
   `g_gameQuestCamLookSpringAxisConsts` +0x40) is built, `lookParams.y` added to its y, and the
   offset's xyz added to `goal` (goal.w keeps the owner's w). The inlined static-zero guard
   (`g_staticZeroFGuard`/`g_staticZeroF`) is only written; the compares use 0.0f directly. */

void GameQuestCamLookSpringUpdateTarget(GameQuestCamLookSpring *self)
{
  ScePspFVector4 dir;
  ScePspFVector4 offset;
  ScePspFVector4 side;
  const ScePspFVector4 *up;
  float *ownerPos;
  float mag;
  float lenSq;
  float scale;

  ownerPos = GameQuestCamSpringGetOwnerPos(&self->base);
  self->base.goal.x = ownerPos[0];
  self->base.goal.y = ownerPos[1];
  self->base.goal.z = ownerPos[2];
  self->base.goal.w = ownerPos[3];
  dir = self->base.ctrl->lookAt;
  dir.x = dir.x - self->base.ctrl->eye.x;
  dir.y = dir.y - self->base.ctrl->eye.y;
  dir.z = dir.z - self->base.ctrl->eye.z;
  dir.y = 0.0f;
  if (g_staticZeroFGuard == 0) {
    g_staticZeroFGuard = 1;
    g_staticZeroF = 0.0f;
  }
  mag = dir.x;
  if (mag < 0.0f) {
    mag = -mag;
  }
  if (mag <= 0.0001f) {
    if (g_staticZeroFGuard == 0) {
      g_staticZeroFGuard = 1;
      g_staticZeroF = 0.0f;
    }
    mag = dir.z;
    if (mag < 0.0f) {
      mag = -mag;
    }
    if (mag <= 0.0001f) {
      return;
    }
  }
  /* normalise, saturated to [-1, 1] */
  lenSq = dir.x * dir.x + dir.y * dir.y + dir.z * dir.z;
  scale = VfRsq(lenSq);
  if (lenSq == 0.0f) {
    scale = 0.0f;
  }
  dir.x = VfSat1(dir.x * scale);
  dir.y = VfSat1(dir.y * scale);
  dir.z = VfSat1(dir.z * scale);
  /* forward part */
  offset.x = dir.x * self->lookParams.z;
  offset.y = dir.y * self->lookParams.z;
  offset.z = dir.z * self->lookParams.z;
  /* lateral part: cross(dir, up) */
  up = &g_gameQuestCamLookSpringAxisConsts.up;
  side.x = dir.y * up->z - dir.z * up->y;
  side.y = dir.z * up->x - dir.x * up->z;
  side.z = dir.x * up->y - dir.y * up->x;
  side.x = side.x * self->lookParams.x;
  side.y = side.y * self->lookParams.x;
  side.z = side.z * self->lookParams.x;
  offset.x = offset.x + side.x;
  offset.y = offset.y + side.y;
  offset.z = offset.z + side.z;
  offset.y = offset.y + self->lookParams.y;
  self->base.goal.x = self->base.goal.x + offset.x;
  self->base.goal.y = self->base.goal.y + offset.y;
  self->base.goal.z = self->base.goal.z + offset.z;
}
