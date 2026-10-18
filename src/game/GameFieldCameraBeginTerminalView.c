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
  offset.x = VfMul(VfCosQuarter(VfMul(cam->yaw, 0.636619747f)), dist);
  offset.y = VfMul(0.0f, dist);
  offset.z = VfMul(VfSinQuarter(VfMul(cam->yaw, 0.636619747f)), dist);
  offset.w = 0.0f;

  /* Horizontal perpendicular, normalised. */
  axis.x = offset.z;
  axis.y = offset.y;
  axis.z = -offset.x;
  len2 = VfDot3(axis.x, axis.y, axis.z, axis.x, axis.y, axis.z);
  k = VfIsZero(len2) ? 0.0f : VfRsq(len2);
  axis.x = VfSat1(VfMul(axis.x, k));
  axis.y = VfSat1(VfMul(axis.y, k));
  axis.z = VfSat1(VfMul(axis.z, k));

  /* Axis-angle quaternion, half angle (1/pi * 0.5235988) quarter turns. */
  half = 0.318309873f * 0.5235988f;
  c = VfCosQuarter(half);
  s = VfSinQuarter(half);
  quat.x = VfMul(axis.x, s);
  quat.y = VfMul(axis.y, s);
  quat.z = VfMul(axis.z, s);
  quat.w = c;
  conj.x = -quat.x;
  conj.y = -quat.y;
  conj.z = -quat.z;
  conj.w = quat.w;

  /* rotated = quat * offset * conj (offset.w = 0). */
  tmp.x = VfDot4(quat.x, quat.y, quat.z, quat.w, offset.w, offset.z, -offset.y, offset.x);
  tmp.y = VfDot4(-quat.x, quat.y, quat.z, quat.w, offset.z, offset.w, offset.x, offset.y);
  tmp.z = VfDot4(quat.x, quat.y, quat.z, quat.w, offset.y, -offset.x, offset.w, offset.z);
  tmp.w = VfDot4(-quat.x, quat.y, quat.z, quat.w, offset.x, -offset.y, -offset.z, offset.w);
  rotated.x = VfDot4(tmp.x, tmp.y, tmp.z, tmp.w, conj.w, conj.z, -conj.y, conj.x);
  rotated.y = VfDot4(-tmp.x, tmp.y, tmp.z, tmp.w, conj.z, conj.w, conj.x, conj.y);
  rotated.z = VfDot4(tmp.x, tmp.y, tmp.z, tmp.w, conj.y, -conj.x, conj.w, conj.z);

  cam->base.eye[0] = VfAdd(cam->followLookAt.x, rotated.x);
  cam->base.eye[1] = VfAdd(cam->followLookAt.y, rotated.y);
  cam->base.eye[2] = VfAdd(cam->followLookAt.z, rotated.z);
  cam->base.eye[3] = cam->followLookAt.w;
  cam->base.target[0] = cam->followLookAt.x;
  cam->base.target[1] = cam->followLookAt.y;
  cam->base.target[2] = cam->followLookAt.z;
  cam->base.target[3] = cam->followLookAt.w;
}
