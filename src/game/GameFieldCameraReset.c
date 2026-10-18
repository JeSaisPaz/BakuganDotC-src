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
  offset.x = VfMul(VfCosQuarter(VfMul(yaw, 0.636619747f)), dist);
  offset.y = VfMul(0.0f, dist);
  offset.z = VfMul(VfSinQuarter(VfMul(yaw, 0.636619747f)), dist);
  offset.w = 0.0f;
  /* horizontal perpendicular, normalised and clamped */
  axis.x = offset.z;
  axis.y = offset.y;
  axis.z = -offset.x;
  lenSq = VfDot3(axis.x, axis.y, axis.z, axis.x, axis.y, axis.z);
  if (VfIsZero(lenSq)) {
    invLen = 0.0f;
  }
  else {
    invLen = VfRsq(lenSq);
  }
  axis.x = VfSat1(VfMul(axis.x, invLen));
  axis.y = VfSat1(VfMul(axis.y, invLen));
  axis.z = VfSat1(VfMul(axis.z, invLen));
  axis.w = 0.0f;
  /* quaternion of -0.12217305 rad about the axis (half angle in quarter turns) */
  halfTurns = 0.318309873f * -0.12217305f;
  s = VfSinQuarter(halfTurns);
  rot.x = VfMul(axis.x, s);
  rot.y = VfMul(axis.y, s);
  rot.z = VfMul(axis.z, s);
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
    q.x = VfDot4(rot.x, rot.y, rot.z, rot.w, offset.w, offset.z, -offset.y, offset.x);
    q.y = VfDot4(-rot.x, rot.y, rot.z, rot.w, offset.z, offset.w, offset.x, offset.y);
    q.z = VfDot4(rot.x, rot.y, rot.z, rot.w, offset.y, -offset.x, offset.w, offset.z);
    q.w = VfDot4(-rot.x, rot.y, rot.z, rot.w, offset.x, -offset.y, -offset.z, offset.w);
    rotated.x = VfDot4(q.x, q.y, q.z, q.w, conj.w, conj.z, -conj.y, conj.x);
    rotated.y = VfDot4(-q.x, q.y, q.z, q.w, conj.z, conj.w, conj.x, conj.y);
    rotated.z = VfDot4(q.x, q.y, q.z, q.w, conj.y, -conj.x, conj.w, conj.z);
    eye.x = VfAdd(cam->followLookAt.x, rotated.x);
    eye.y = VfAdd(cam->followLookAt.y, rotated.y);
    eye.z = VfAdd(cam->followLookAt.z, rotated.z);
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
