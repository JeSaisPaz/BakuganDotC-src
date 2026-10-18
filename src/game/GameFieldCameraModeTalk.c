// bdc 0x088bcf38 GameFieldCameraModeTalk
#include "bdc.h"

/* Mode 1 (talk view) of the field camera (embedded at `+0x20` of the field scene task,
   `GameFieldUpdate`). Does nothing without a target or with `stepState` outside 0..2; state 1 returns
   unchanged and state 2 only advances `stepState`. In state 0 it saves `followLookAt` into `followGoal`, eases
   `followLookAt` 10% towards `talkLookAt` raised by `g_gameFieldCameraTalkLookAtHeight`, adds 4 to
   `talkBlend`, decays `pitch` by 20%, then rebuilds the eye: the distance eases 10% towards
   `talkEye.y`, the elevation 20% from the current one towards `-pitch` and, while `talkBlend` > 0,
   `yawVel` is steered by the wrapped yaw error weighted by a cosine ramp of `talkBlend * 0.05`
   (else it decays by 20%). The horizontal offset at yaw `atan2f + yawVel` is rotated about its
   horizontal perpendicular by the elevation (`MathQuatFromAxisAngle`) and added to
   `followLookAt` to give `eye`; `target` = `followLookAt`. Once the yaw error and the distance
   error are both below 0.001, `stepState` advances. */

void GameFieldCameraModeTalk(GameFieldCamera *cam)

{
  ScePspFVector4 look;
  ScePspFVector4 offset;
  ScePspFVector4 axis;
  ScePspFVector4 rot;
  ScePspFVector4 tmp;
  ScePspFVector4 rotated;
  ScePspFVector4 q;
  ScePspFVector4 qc;
  ScePspFVector4 qv;
  float *quat;
  float dist;
  float radius;
  float ramp;
  float cosRamp;
  float yaw;
  float len;
  float elev;
  float yawTarget;
  float yawErr;
  float wrap;
  float vel;

  if (cam->target == NULL) {
    return;
  }
  if (cam->stepState >= 2) {
    if (cam->stepState < 3) {
      cam->stepState = cam->stepState + 1;
    }
    return;
  }
  if (cam->stepState < 0 || cam->stepState > 0) {
    return;
  }
  look = cam->talkLookAt;
  look.y = look.y + g_gameFieldCameraTalkLookAtHeight;
  cam->followGoal = cam->followLookAt;
  /* followLookAt += (look - followLookAt) * 0.1 (all four lanes) */
  cam->followLookAt.x = VfAdd(cam->followLookAt.x, VfMul(VfSub(look.x, cam->followLookAt.x), 0.1f));
  cam->followLookAt.y = VfAdd(cam->followLookAt.y, VfMul(VfSub(look.y, cam->followLookAt.y), 0.1f));
  cam->followLookAt.z = VfAdd(cam->followLookAt.z, VfMul(VfSub(look.z, cam->followLookAt.z), 0.1f));
  cam->followLookAt.w = VfAdd(cam->followLookAt.w, VfMul(VfSub(look.w, cam->followLookAt.w), 0.1f));
  cam->talkBlend = cam->talkBlend + 4;
  cam->pitch = cam->pitch + cam->pitch * -0.2f;
  /* offset = eye - followLookAt (xyz, w = eye.w); dist = |offset.xyz| */
  tmp.x = VfSub(cam->base.eye[0], cam->followLookAt.x);
  tmp.y = VfSub(cam->base.eye[1], cam->followLookAt.y);
  tmp.z = VfSub(cam->base.eye[2], cam->followLookAt.z);
  tmp.w = cam->base.eye[3];
  offset = tmp;
  dist = VfSqrt(VfDot3(offset.x, offset.y, offset.z, offset.x, offset.y, offset.z));
  radius = dist + (cam->talkEye.y - dist) * 0.1f;
  elev = -atan2f(offset.y, __builtin_sqrtf(offset.x * offset.x + offset.z * offset.z));
  elev = elev + (-cam->pitch - elev) * 0.2f;
  yawTarget = atan2f(offset.z, offset.x);
  yawErr = 0.0f;
  if (cam->talkBlend > 0) {
    ramp = (float)cam->talkBlend * 0.05f;
    if (!(ramp <= 1.0f)) {
      ramp = 1.0f;
    }
    wrap = yawTarget - cam->yaw;
    wrap = wrap - (float)(s32)(wrap * 0.318309873f) * 6.28318548f;
    if (wrap < 0.0f) {
      wrap = wrap + 6.28318548f;
    }
    if (wrap < 3.14159274f) {
      wrap = -wrap;
    }
    else {
      wrap = 6.28318548f - wrap;
    }
    yawErr = wrap;
    ramp = ramp * ramp * 3.14159274f;
    cosRamp = VfCosQuarter(VfMul(ramp, 0.636619747f));
    vel = cam->yawVel;
    wrap = vel - yawErr * ((1.0f - cosRamp) * 0.5f) * 0.1f;
    wrap = wrap - (float)(s32)(wrap * 0.318309873f) * 6.28318548f;
    if (wrap < 0.0f) {
      wrap = wrap + 6.28318548f;
    }
    if (wrap < 3.14159274f) {
      wrap = -wrap;
    }
    else {
      wrap = 6.28318548f - wrap;
    }
    cam->yawVel = vel + wrap * 0.2f;
  }
  else {
    cam->yawVel = cam->yawVel * 0.8f;
  }
  yaw = yawTarget + cam->yawVel;
  /* offset = (cos yaw, 0, sin yaw, 0) * radius (w not scaled) */
  offset.x = VfMul(VfCosQuarter(VfMul(yaw, 0.636619747f)), radius);
  offset.y = VfMul(0.0f, radius);
  offset.z = VfMul(VfSinQuarter(VfMul(yaw, 0.636619747f)), radius);
  offset.w = 0.0f;
  /* axis = normalize(offset.z, offset.y, -offset.x), each lane clamped to [-1, 1]; w = 0 */
  axis.x = offset.z;
  axis.y = offset.y;
  axis.z = -offset.x;
  len = VfDot3(axis.x, axis.y, axis.z, axis.x, axis.y, axis.z);
  len = VfIsZero(len) ? 0.0f : VfRsq(len);
  axis.x = VfSat1(VfMul(axis.x, len));
  axis.y = VfSat1(VfMul(axis.y, len));
  axis.z = VfSat1(VfMul(axis.z, len));
  axis.w = 0.0f;
  quat = MathQuatFromAxisAngle(elev, (float *)&rot, (const float *)&axis);
  /* rotated = quat * (offset.xyz, 0) * conj(quat) */
  q.x = quat[0];
  q.y = quat[1];
  q.z = quat[2];
  q.w = quat[3];
  qc.x = -q.x;
  qc.y = -q.y;
  qc.z = -q.z;
  qc.w = q.w;
  qv.x = VfDot4(q.x, q.y, q.z, q.w, offset.w, offset.z, -offset.y, offset.x);
  qv.y = VfDot4(-q.x, q.y, q.z, q.w, offset.z, offset.w, offset.x, offset.y);
  qv.z = VfDot4(q.x, q.y, q.z, q.w, offset.y, -offset.x, offset.w, offset.z);
  qv.w = VfDot4(-q.x, q.y, q.z, q.w, offset.x, -offset.y, -offset.z, offset.w);
  rotated.x = VfDot4(qv.x, qv.y, qv.z, qv.w, qc.w, qc.z, -qc.y, qc.x);
  rotated.y = VfDot4(-qv.x, qv.y, qv.z, qv.w, qc.z, qc.w, qc.x, qc.y);
  rotated.z = VfDot4(qv.x, qv.y, qv.z, qv.w, qc.y, -qc.x, qc.w, qc.z);
  /* eye = followLookAt + rotated (xyz, w = followLookAt.w); target = followLookAt */
  tmp.x = VfAdd(cam->followLookAt.x, rotated.x);
  tmp.y = VfAdd(cam->followLookAt.y, rotated.y);
  tmp.z = VfAdd(cam->followLookAt.z, rotated.z);
  tmp.w = cam->followLookAt.w;
  cam->base.eye[0] = tmp.x;
  cam->base.eye[1] = tmp.y;
  cam->base.eye[2] = tmp.z;
  cam->base.eye[3] = tmp.w;
  cam->base.target[0] = cam->followLookAt.x;
  cam->base.target[1] = cam->followLookAt.y;
  cam->base.target[2] = cam->followLookAt.z;
  cam->base.target[3] = cam->followLookAt.w;
  if (__builtin_fabsf(yawErr) < 0.001f) {
    /* dist = |eye - followLookAt| */
    rotated.x = VfSub(cam->base.eye[0], cam->followLookAt.x);
    rotated.y = VfSub(cam->base.eye[1], cam->followLookAt.y);
    rotated.z = VfSub(cam->base.eye[2], cam->followLookAt.z);
    rotated.w = cam->base.eye[3];
    dist = VfSqrt(VfDot3(rotated.x, rotated.y, rotated.z, rotated.x, rotated.y, rotated.z));
    if (__builtin_fabsf(cam->talkEye.y - dist) < 0.001f) {
      cam->stepState = cam->stepState + 1;
    }
  }
}
