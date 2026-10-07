// bdc 0x088ba98c GameFieldCameraBeginTerminalView
#include "bdc.h"

/* Switches the field camera (`GameFieldCameraCtor`, embedded at `+0x20` of the field task) to mode 2
   (`GameFieldCameraModeTerminal`): clears `talkBlend`/`stepState`, pitch -30 degrees, yaw = target
   heading + pi (wrapped to (-pi, pi]), look-at `followLookAt`/`followGoal` = target position raised by
   `g_gameFieldCameraTerminalLookAtHeight`, and places the eye at once: the yaw offset of length
   `g_gameFieldCameraTerminalDistance` rotated by a +30 degree quaternion (half angle 1/6 quarter turn)
   about its normalised horizontal perpendicular, added to the look-at; `base.target` = the look-at. */

void GameFieldCameraBeginTerminalView(GameFieldCamera *cam)

{
  Actor *target;
  ScePspFVector4 offset;
  ScePspFVector4 axis;
  ScePspFVector4 quat;
  ScePspFVector4 conj;
  ScePspFVector4 tmp;
  ScePspFVector4 rotated;
  float dist;
  float len2;
  float k;
  float half;
  float s;
  float c;

  GameFieldCameraSetMode(cam, 2);
  cam->talkBlend = 0;
  cam->stepState = 0;
  cam->pitch = -0.5235988f;
  target = (Actor *)cam->target;
  cam->yaw = target->base.rot[1] + 3.1415927f;
  if (!(cam->yaw <= 3.1415927f)) {
    cam->yaw = cam->yaw - 6.2831855f;
  }
  else if (cam->yaw <= -3.1415927f) {
    cam->yaw = cam->yaw + 6.2831855f;
  }
  target = (Actor *)cam->target;
  cam->followLookAt = *(ScePspFVector4 *)target->base.pos;
  cam->followLookAt.y = cam->followLookAt.y + g_gameFieldCameraTerminalLookAtHeight;
  cam->followGoal = cam->followLookAt;

  /* Offset in the yaw direction, length dist. */
  dist = g_gameFieldCameraTerminalDistance;
  offset.x = __builtin_cosf(cam->yaw) * dist;
  offset.y = 0.0f * dist;
  offset.z = __builtin_sinf(cam->yaw) * dist;
  offset.w = 0.0f;

  /* Horizontal perpendicular, normalised. */
  axis.x = offset.z;
  axis.y = offset.y;
  axis.z = -offset.x;
  len2 = axis.x * axis.x + axis.y * axis.y + axis.z * axis.z;
  k = (len2 == 0.0f) ? 0.0f : VfRsq(len2);
  axis.x = VfSat1(axis.x * k);
  axis.y = VfSat1(axis.y * k);
  axis.z = VfSat1(axis.z * k);

  /* Axis-angle quaternion, half angle (1/pi * 0.5235988) quarter turns. */
  half = 0.318309873f * 0.5235988f;
  c = VfCosQuarter(half);
  s = VfSinQuarter(half);
  quat.x = axis.x * s;
  quat.y = axis.y * s;
  quat.z = axis.z * s;
  quat.w = c;
  conj.x = -quat.x;
  conj.y = -quat.y;
  conj.z = -quat.z;
  conj.w = quat.w;

  /* rotated = quat * offset * conj (offset.w = 0). */
  tmp.x = quat.x * offset.w + quat.y * offset.z - quat.z * offset.y + quat.w * offset.x;
  tmp.y = -quat.x * offset.z + quat.y * offset.w + quat.z * offset.x + quat.w * offset.y;
  tmp.z = quat.x * offset.y - quat.y * offset.x + quat.z * offset.w + quat.w * offset.z;
  tmp.w = -quat.x * offset.x - quat.y * offset.y - quat.z * offset.z + quat.w * offset.w;
  rotated.x = tmp.x * conj.w + tmp.y * conj.z - tmp.z * conj.y + tmp.w * conj.x;
  rotated.y = -tmp.x * conj.z + tmp.y * conj.w + tmp.z * conj.x + tmp.w * conj.y;
  rotated.z = tmp.x * conj.y - tmp.y * conj.x + tmp.z * conj.w + tmp.w * conj.z;

  cam->base.eye[0] = cam->followLookAt.x + rotated.x;
  cam->base.eye[1] = cam->followLookAt.y + rotated.y;
  cam->base.eye[2] = cam->followLookAt.z + rotated.z;
  cam->base.eye[3] = cam->followLookAt.w;
  cam->base.target[0] = cam->followLookAt.x;
  cam->base.target[1] = cam->followLookAt.y;
  cam->base.target[2] = cam->followLookAt.z;
  cam->base.target[3] = cam->followLookAt.w;
}
