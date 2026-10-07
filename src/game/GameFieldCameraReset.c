// bdc 0x088b9f14 GameFieldCameraReset
#include "bdc.h"

/* Snaps the field camera (`GameFieldCameraCtor`, embedded at `+0x20` of the field scene task; follow
   target `+0x2a0`, eye `+0x50`, look-at `+0x60`, mode `+0x2ac`) behind its target in mode 0: distance
   `g_gameFieldCameraDefaultDistance`, yaw = target heading + pi (wrapped to (-pi, pi]), pitch 0.1222,
   look-at `followLookAt`/`followGoal` = target position raised by `g_gameFieldCameraLookAtHeight`,
   resets the quest camera controller `questCam` (`GameQuestCamCtrlReset`) when there is one. With
   `snap` the eye is placed at once (the yaw offset vector rotated by -0.1222 rad about its horizontal
   perpendicular, added to the look-at) and `pitchVel` = 1, else `pitchVel` = 0. With `keepMode`
   `talkBlend` = 0, `pitchVel` = 1 and `stepState` = 1, else `stepState` = 0.
   The yaw offset is `(cos yaw, 0, sin yaw) * distance`; its horizontal perpendicular `(z, y, -x)` is
   normalised (0 for a zero vector, clamped to [-1, 1]) into the rotation axis of the quaternion. */

void GameFieldCameraReset(GameFieldCamera *cam, char snap, char keepMode)

{
  Actor *target;
  ScePspFVector4 rot;
  ScePspFVector4 offset;
  ScePspFVector4 axis;
  ScePspFVector4 conj;
  ScePspFVector4 q;
  ScePspFVector4 rotated;
  ScePspFVector4 eye;
  float yaw;
  float dist;
  float lenSq;
  float invLen;
  float halfTurns;
  float s;

  cam->distance = g_gameFieldCameraDefaultDistance;
  cam->stepState = 0;
  GameFieldCameraSetMode(cam, 0);
  cam->yawVel = 0.0f;
  cam->pitch = 0.12217305f;
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
  cam->followLookAt.y = cam->followLookAt.y + g_gameFieldCameraLookAtHeight;
  cam->followGoal = cam->followLookAt;
  yaw = cam->yaw;
  dist = g_gameFieldCameraDefaultDistance;
  /* vrot [C,0,S,0] of yaw * 2/pi quarter turns, scaled by the distance */
  offset.x = __builtin_cosf(yaw) * dist;
  offset.y = 0.0f * dist;
  offset.z = __builtin_sinf(yaw) * dist;
  offset.w = 0.0f;
  /* horizontal perpendicular, normalised and clamped */
  axis.x = offset.z;
  axis.y = offset.y;
  axis.z = -offset.x;
  lenSq = axis.x * axis.x + axis.y * axis.y + axis.z * axis.z;
  if (lenSq == 0.0f) {
    invLen = 0.0f;
  }
  else {
    invLen = VfRsq(lenSq);
  }
  axis.x = VfSat1(axis.x * invLen);
  axis.y = VfSat1(axis.y * invLen);
  axis.z = VfSat1(axis.z * invLen);
  axis.w = 0.0f;
  /* quaternion of -0.12217305 rad about the axis (half angle in quarter turns) */
  halfTurns = 0.318309873f * -0.12217305f;
  s = VfSinQuarter(halfTurns);
  rot.x = axis.x * s;
  rot.y = axis.y * s;
  rot.z = axis.z * s;
  rot.w = VfCosQuarter(halfTurns);
  if (cam->questCam != NULL) {
    GameQuestCamCtrlReset((GameQuestCamCtrl *)cam->questCam);
  }
  if (snap != 0) {
    /* rotated = rot * (offset.xyz, 0) * conj(rot); eye = followLookAt + rotated (xyz) */
    conj.x = -rot.x;
    conj.y = -rot.y;
    conj.z = -rot.z;
    conj.w = rot.w;
    offset.w = 0.0f;
    q.x = rot.x * offset.w + rot.y * offset.z - rot.z * offset.y + rot.w * offset.x;
    q.y = -rot.x * offset.z + rot.y * offset.w + rot.z * offset.x + rot.w * offset.y;
    q.z = rot.x * offset.y - rot.y * offset.x + rot.z * offset.w + rot.w * offset.z;
    q.w = -rot.x * offset.x - rot.y * offset.y - rot.z * offset.z + rot.w * offset.w;
    rotated.x = q.x * conj.w + q.y * conj.z - q.z * conj.y + q.w * conj.x;
    rotated.y = -q.x * conj.z + q.y * conj.w + q.z * conj.x + q.w * conj.y;
    rotated.z = q.x * conj.y - q.y * conj.x + q.z * conj.w + q.w * conj.z;
    eye.x = cam->followLookAt.x + rotated.x;
    eye.y = cam->followLookAt.y + rotated.y;
    eye.z = cam->followLookAt.z + rotated.z;
    eye.w = cam->followLookAt.w;
    *(ScePspFVector4 *)cam->base.eye = eye;
    *(ScePspFVector4 *)cam->base.target = cam->followLookAt;
    cam->pitchVel = 1.0f;
  }
  else {
    cam->pitchVel = 0.0f;
  }
  if (keepMode != 0) {
    cam->talkBlend = 0;
    cam->pitchVel = 1.0f;
    cam->stepState = 1;
  }
  else {
    cam->stepState = 0;
  }
}
