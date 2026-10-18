// bdc 0x088bb5e4 GameFieldCameraFollowStep
#include "bdc.h"

/* Mode 3 (step follow) of the field camera (embedded at `+0x20` of the field scene task,
   `GameFieldUpdate`; started by `GameFieldCameraBeginStepFollow`). Does nothing without a target
   or when `stepState` is negative or above 1.
   `stepState` 0 (swing behind): `followGoal` keeps the old `followLookAt`, which eases 35% towards
   the target position raised by `g_gameFieldCameraLookAtHeight`; `yaw` eases 30% towards the
   target heading + pi (wrapped to (-pi, pi]) and `pitch` 20% towards 0.1222. The orbit radius eases
   40% from the eye distance towards `distance`, the elevation 20% towards `-pitch`. The wrapped
   heading error scaled by 0.3 (times `(1 - cos(pi * S703)) / 2`) drives `yawVel`: below 0.01 it is
   zeroed and `stepState` advances by 2 (by 1, with `pitchVel` zeroed, when target `motion` flag
   `0x1000000` is set); up to 0.04 it halves with a 0.03 magnitude floor; else it eases 20% towards
   the error, clamped to +-0.8.
   `stepState` 1 (free follow): advances to 2 unless `motion` flag `0x1000000` is set; the look-at
   goal is the target position 8 units behind the generic camera's yaw, raised by
   `g_gameFieldCameraHighLookAtHeight`; `yaw` eases as above; stick Y moves `pitch` by 0.06 within
   +-0.61086524; the radius eases 40% towards `g_gameFieldCameraStepDistance`; `motion` flag
   `0x100000` or stick X > 0.8 spins `yawVel` down by 0.01 (floor -0.1), `0x200000` or stick X < -0.8
   up by 0.01 (cap 0.1), else it decays by 25%; while still in state 1 the target's heading is set to
   face away from the camera.
   Both: the eye orbits the look-at at the eased radius, yaw `heading + yawVel` and the eased
   elevation; `pitchVel` grows by 0.1 up to 1 and its square blends eye and look-at
   (`MathQuatFromAxisAngle` or its inlined form), then walls are resolved
   (`GameFieldCameraAvoidWalls`). Bank constants read: S703 (2/pi, so the quarter-turn trig takes
   radians), S713 (0: zero-length axis and the axis's w) and S730 (0: w of the rotated vector). */

void GameFieldCameraFollowStep(GameFieldCamera *cam)

{
  ScePspFVector4 look;
  ScePspFVector4 offset;
  ScePspFVector4 axis;
  ScePspFVector4 rot;
  ScePspFVector4 tmp;
  ScePspFVector4 mid;
  ScePspFVector4 rotated;
  Actor *target;
  float *quat;
  float dist;
  float radius;
  float yaw;
  float blend;
  float angle;
  float k;
  float s;
  float behindX;
  float behindZ;
  float elev;
  float heading;
  float swing;
  float stickX;
  float d;
  float v;

  if (cam->target == NULL) {
    return;
  }
  if (cam->stepState > 0) {
    if (cam->stepState >= 2) {
      return;
    }
    /* ---- stepState 1: free follow ---- */
    if ((((Actor *)cam->target)->motion & 0x1000000) == 0) {
      cam->stepState = cam->stepState + 1;
    }
    look = *(ScePspFVector4 *)((Actor *)cam->target)->base.pos;
    yaw = 1.5707964f - cam->base.yaw;
    if (!(yaw <= 3.1415927f)) {
      yaw = yaw - 6.2831855f;
    }
    else if (yaw <= -3.1415927f) {
      yaw = yaw + 6.2831855f;
    }
    /* behind = (cos yaw, 0, sin yaw) * -8 */
    behindX = VfMul(VfCosQuarter(VfMul(yaw, 0.636619747f)), -8.0f);
    behindZ = VfMul(VfSinQuarter(VfMul(yaw, 0.636619747f)), -8.0f);
    look.x = look.x + behindX;
    look.z = look.z + behindZ;
    look.y = look.y + g_gameFieldCameraHighLookAtHeight;
    cam->followGoal = cam->followLookAt;
    /* followLookAt += (look - followLookAt) * 0.35 */
    cam->followLookAt.x = VfAdd(cam->followLookAt.x, VfMul(VfSub(look.x, cam->followLookAt.x), 0.35f));
    cam->followLookAt.y = VfAdd(cam->followLookAt.y, VfMul(VfSub(look.y, cam->followLookAt.y), 0.35f));
    cam->followLookAt.z = VfAdd(cam->followLookAt.z, VfMul(VfSub(look.z, cam->followLookAt.z), 0.35f));
    cam->followLookAt.w = VfAdd(cam->followLookAt.w, VfMul(VfSub(look.w, cam->followLookAt.w), 0.35f));
    d = cam->yaw - (((Actor *)cam->target)->base.rot[1] + 3.1415927f);
    d = d - (float)(s32)(d * 0.31830987f) * 6.2831855f;
    if (d < 0.0f) {
      d = d + 6.2831855f;
    }
    if (d < 3.1415927f) {
      d = -d;
    }
    else {
      d = 6.2831855f - d;
    }
    v = cam->yaw + d * 0.3f;
    cam->yaw = v;
    if (!(v <= 3.1415927f)) {
      cam->yaw = cam->yaw - 6.2831855f;
    }
    else if (cam->yaw <= -3.1415927f) {
      cam->yaw = cam->yaw + 6.2831855f;
    }
    if (g_padState->stickY < -0.8f) {
      v = cam->pitch - 0.06f;
      if (v < -0.61086524f) {
        v = -0.61086524f;
      }
      cam->pitch = v;
    }
    else if (!(g_padState->stickY <= 0.8f)) {
      v = cam->pitch + 0.06f;
      if (!(v <= 0.61086524f)) {
        v = 0.61086524f;
      }
      cam->pitch = v;
    }
    /* offset = eye - followLookAt (xyz, w = eye.w); dist = |offset.xyz| */
    offset.x = VfSub(cam->base.eye[0], cam->followLookAt.x);
    offset.y = VfSub(cam->base.eye[1], cam->followLookAt.y);
    offset.z = VfSub(cam->base.eye[2], cam->followLookAt.z);
    offset.w = cam->base.eye[3];
    dist = VfSqrt(VfDot3(offset.x, offset.y, offset.z, offset.x, offset.y, offset.z));
    radius = dist + (g_gameFieldCameraStepDistance - dist) * 0.4f;
    elev = -atan2f(offset.y, __builtin_sqrtf(offset.x * offset.x + offset.z * offset.z));
    angle = elev + (-cam->pitch - elev) * 0.2f;
    heading = atan2f(offset.z, offset.x);
    stickX = g_padState->stickX;
    if ((((Actor *)cam->target)->motion & 0x100000) != 0 || !(stickX <= 0.8f)) {
      v = cam->yawVel - 0.01f;
      if (v < -0.1f) {
        v = -0.1f;
      }
      cam->yawVel = v;
    }
    else {
      if ((((Actor *)cam->target)->motion & 0x200000) != 0 || stickX < -0.8f) {
        v = cam->yawVel + 0.01f;
        if (!(v <= 0.1f)) {
          v = 0.1f;
        }
      }
      else {
        v = cam->yawVel * 0.75f;
      }
      cam->yawVel = v;
    }
    yaw = heading + v;
    /* offset = (cos yaw, 0, sin yaw, 0) scaled (xyz) by radius; axis = (offset.z, offset.y, -offset.x, offset.w) */
    offset.x = VfMul(VfCosQuarter(VfMul(yaw, 0.636619747f)), radius);
    offset.y = VfMul(0.0f, radius);
    offset.z = VfMul(VfSinQuarter(VfMul(yaw, 0.636619747f)), radius);
    offset.w = 0.0f;
    axis.x = offset.z;
    axis.y = offset.y;
    axis.z = -offset.x;
    axis.w = offset.w;
    if (cam->stepState == 1) {
      target = (Actor *)cam->target;
      target->base.rot[1] = atan2f(offset.z, offset.x) + 3.1415927f;
    }
    blend = cam->pitchVel + 0.1f;
    if (!(blend <= 1.0f)) {
      blend = 1.0f;
    }
    cam->pitchVel = blend;
    blend = blend * blend;
    /* axis = normalize(axis), clamped to [-1, 1]; a zero-length axis scales by 0 (S713); w = S713 (0) */
    k = VfDot3(axis.x, axis.y, axis.z, axis.x, axis.y, axis.z);
    if (VfIsZero(k)) {
      k = 0.0f;
    }
    else {
      k = VfRsq(k);
    }
    axis.x = VfSat1(VfMul(axis.x, k));
    axis.y = VfSat1(VfMul(axis.y, k));
    axis.z = VfSat1(VfMul(axis.z, k));
    axis.w = 0.0f;
    /* rot = (axis * sin(angle/2), cos(angle/2)): angle / pi in quarter turns */
    s = VfMul(angle, 0.318309873f);
    rot.w = VfCosQuarter(s);
    s = VfSinQuarter(s);
    rot.x = VfMul(axis.x, s);
    rot.y = VfMul(axis.y, s);
    rot.z = VfMul(axis.z, s);
    /* rotated = rot * (offset.xyz, 0) * conj(rot) */
    tmp.x = offset.x;
    tmp.y = offset.y;
    tmp.z = offset.z;
    tmp.w = 0.0f;
    mid.x = VfDot4(rot.x, rot.y, rot.z, rot.w, tmp.w, tmp.z, -tmp.y, tmp.x);
    mid.y = VfDot4(-rot.x, rot.y, rot.z, rot.w, tmp.z, tmp.w, tmp.x, tmp.y);
    mid.z = VfDot4(rot.x, rot.y, rot.z, rot.w, tmp.y, -tmp.x, tmp.w, tmp.z);
    mid.w = VfDot4(-rot.x, rot.y, rot.z, rot.w, tmp.x, -tmp.y, -tmp.z, tmp.w);
    rotated.x = VfDot4(mid.x, mid.y, mid.z, mid.w, rot.w, -rot.z, rot.y, -rot.x);
    rotated.y = VfDot4(-mid.x, mid.y, mid.z, mid.w, -rot.z, rot.w, -rot.x, -rot.y);
    rotated.z = VfDot4(mid.x, mid.y, mid.z, mid.w, -rot.y, rot.x, rot.w, -rot.z);
    rotated.w = VfDot4(-mid.x, mid.y, mid.z, mid.w, -rot.x, rot.y, rot.z, rot.w);
    /* tmp = followLookAt + rotated (xyz, w = followLookAt.w) */
    tmp.x = VfAdd(cam->followLookAt.x, rotated.x);
    tmp.y = VfAdd(cam->followLookAt.y, rotated.y);
    tmp.z = VfAdd(cam->followLookAt.z, rotated.z);
    tmp.w = cam->followLookAt.w;
    /* eye += (tmp - eye) * blend; target += (followLookAt - target) * blend */
    cam->base.eye[0] = VfAdd(cam->base.eye[0], VfMul(VfSub(tmp.x, cam->base.eye[0]), blend));
    cam->base.eye[1] = VfAdd(cam->base.eye[1], VfMul(VfSub(tmp.y, cam->base.eye[1]), blend));
    cam->base.eye[2] = VfAdd(cam->base.eye[2], VfMul(VfSub(tmp.z, cam->base.eye[2]), blend));
    cam->base.eye[3] = VfAdd(cam->base.eye[3], VfMul(VfSub(tmp.w, cam->base.eye[3]), blend));
    cam->base.target[0] = VfAdd(cam->base.target[0], VfMul(VfSub(cam->followLookAt.x, cam->base.target[0]), blend));
    cam->base.target[1] = VfAdd(cam->base.target[1], VfMul(VfSub(cam->followLookAt.y, cam->base.target[1]), blend));
    cam->base.target[2] = VfAdd(cam->base.target[2], VfMul(VfSub(cam->followLookAt.z, cam->base.target[2]), blend));
    cam->base.target[3] = VfAdd(cam->base.target[3], VfMul(VfSub(cam->followLookAt.w, cam->base.target[3]), blend));
    GameFieldCameraAvoidWalls(cam);
    return;
  }
  if (cam->stepState < 0) {
    return;
  }
  /* ---- stepState 0: swing behind the target ---- */
  look = *(ScePspFVector4 *)((Actor *)cam->target)->base.pos;
  look.y = look.y + g_gameFieldCameraLookAtHeight;
  cam->followGoal = cam->followLookAt;
  /* followLookAt += (look - followLookAt) * 0.35 */
  cam->followLookAt.x = VfAdd(cam->followLookAt.x, VfMul(VfSub(look.x, cam->followLookAt.x), 0.35f));
  cam->followLookAt.y = VfAdd(cam->followLookAt.y, VfMul(VfSub(look.y, cam->followLookAt.y), 0.35f));
  cam->followLookAt.z = VfAdd(cam->followLookAt.z, VfMul(VfSub(look.z, cam->followLookAt.z), 0.35f));
  cam->followLookAt.w = VfAdd(cam->followLookAt.w, VfMul(VfSub(look.w, cam->followLookAt.w), 0.35f));
  d = cam->yaw - (((Actor *)cam->target)->base.rot[1] + 3.1415927f);
  d = d - (float)(s32)(d * 0.31830987f) * 6.2831855f;
  if (d < 0.0f) {
    d = d + 6.2831855f;
  }
  if (d < 3.1415927f) {
    d = -d;
  }
  else {
    d = 6.2831855f - d;
  }
  v = cam->yaw + d * 0.3f;
  cam->yaw = v;
  if (!(v <= 3.1415927f)) {
    cam->yaw = cam->yaw - 6.2831855f;
  }
  else if (cam->yaw <= -3.1415927f) {
    cam->yaw = cam->yaw + 6.2831855f;
  }
  v = cam->pitch;
  cam->pitch = v + (0.12217305f - v) * 0.2f;
  /* offset = eye - followLookAt (xyz, w = eye.w); dist = |offset.xyz| */
  offset.x = VfSub(cam->base.eye[0], cam->followLookAt.x);
  offset.y = VfSub(cam->base.eye[1], cam->followLookAt.y);
  offset.z = VfSub(cam->base.eye[2], cam->followLookAt.z);
  offset.w = cam->base.eye[3];
  dist = VfSqrt(VfDot3(offset.x, offset.y, offset.z, offset.x, offset.y, offset.z));
  radius = dist + (cam->distance - dist) * 0.4f;
  elev = -atan2f(offset.y, __builtin_sqrtf(offset.x * offset.x + offset.z * offset.z));
  angle = elev + (-cam->pitch - elev) * 0.2f;
  heading = atan2f(offset.z, offset.x);
  d = heading - cam->yaw;
  d = d - (float)(s32)(d * 0.31830987f) * 6.2831855f;
  if (d < 0.0f) {
    d = d + 6.2831855f;
  }
  if (d < 3.1415927f) {
    d = -d;
  }
  else {
    d = 6.2831855f - d;
  }
  /* (1 - cos(pi)) / 2: vcos of pi * S703 (2/pi) quarter turns */
  swing = (1.0f - VfCosQuarter(3.1415927f * 0.636619747f)) * 0.5f * d * 0.3f;
  if (__builtin_fabsf(swing) < 0.01f) {
    cam->yawVel = 0.0f;
    if ((((Actor *)cam->target)->motion & 0x1000000) == 0) {
      cam->stepState = cam->stepState + 2;
    }
    else {
      cam->pitchVel = 0.0f;
      cam->stepState = cam->stepState + 1;
    }
  }
  else if (!(__builtin_fabsf(swing) <= 0.04f)) {
    v = cam->yawVel;
    d = v - swing;
    d = d - (float)(s32)(d * 0.31830987f) * 6.2831855f;
    if (d < 0.0f) {
      d = d + 6.2831855f;
    }
    if (d < 3.1415927f) {
      d = -d;
    }
    else {
      d = 6.2831855f - d;
    }
    v = v + d * 0.2f;
    cam->yawVel = v;
    if (v < 0.0f) {
      v = cam->yawVel;
      if (v < -0.8f) {
        v = -0.8f;
      }
    }
    else {
      v = cam->yawVel;
      if (!(v <= 0.8f)) {
        v = 0.8f;
      }
    }
    cam->yawVel = v;
  }
  else {
    v = cam->yawVel * 0.5f;
    cam->yawVel = v;
    if (v < 0.0f) {
      v = cam->yawVel;
      if (!(v <= -0.03f)) {
        v = -0.03f;
      }
    }
    else {
      v = cam->yawVel;
      if (v < 0.03f) {
        v = 0.03f;
      }
    }
    cam->yawVel = v;
  }
  yaw = heading + cam->yawVel;
  /* offset = (cos yaw, 0, sin yaw, 0) scaled (xyz) by radius; axis = (offset.z, offset.y, -offset.x, offset.w) */
  offset.x = VfMul(VfCosQuarter(VfMul(yaw, 0.636619747f)), radius);
  offset.y = VfMul(0.0f, radius);
  offset.z = VfMul(VfSinQuarter(VfMul(yaw, 0.636619747f)), radius);
  offset.w = 0.0f;
  axis.x = offset.z;
  axis.y = offset.y;
  axis.z = -offset.x;
  axis.w = offset.w;
  blend = cam->pitchVel + 0.1f;
  if (!(blend <= 1.0f)) {
    blend = 1.0f;
  }
  cam->pitchVel = blend;
  blend = blend * blend;
  /* axis = normalize(axis), clamped to [-1, 1]; a zero-length axis scales by 0 (S713); w = S713 (0) */
  k = VfDot3(axis.x, axis.y, axis.z, axis.x, axis.y, axis.z);
  if (VfIsZero(k)) {
    k = 0.0f;
  }
  else {
    k = VfRsq(k);
  }
  axis.x = VfSat1(VfMul(axis.x, k));
  axis.y = VfSat1(VfMul(axis.y, k));
  axis.z = VfSat1(VfMul(axis.z, k));
  axis.w = 0.0f;
  quat = MathQuatFromAxisAngle(angle, (float *)&rot, (const float *)&axis);
  /* rotated = quat * (offset.xyz, 0) * conj(quat) */
  tmp.x = offset.x;
  tmp.y = offset.y;
  tmp.z = offset.z;
  tmp.w = 0.0f;
  mid.x = VfDot4(quat[0], quat[1], quat[2], quat[3], tmp.w, tmp.z, -tmp.y, tmp.x);
  mid.y = VfDot4(-quat[0], quat[1], quat[2], quat[3], tmp.z, tmp.w, tmp.x, tmp.y);
  mid.z = VfDot4(quat[0], quat[1], quat[2], quat[3], tmp.y, -tmp.x, tmp.w, tmp.z);
  mid.w = VfDot4(-quat[0], quat[1], quat[2], quat[3], tmp.x, -tmp.y, -tmp.z, tmp.w);
  rotated.x = VfDot4(mid.x, mid.y, mid.z, mid.w, quat[3], -quat[2], quat[1], -quat[0]);
  rotated.y = VfDot4(-mid.x, mid.y, mid.z, mid.w, -quat[2], quat[3], -quat[0], -quat[1]);
  rotated.z = VfDot4(mid.x, mid.y, mid.z, mid.w, -quat[1], quat[0], quat[3], -quat[2]);
  rotated.w = VfDot4(-mid.x, mid.y, mid.z, mid.w, -quat[0], quat[1], quat[2], quat[3]);
  /* tmp = followLookAt + rotated (xyz, w = followLookAt.w) */
  tmp.x = VfAdd(cam->followLookAt.x, rotated.x);
  tmp.y = VfAdd(cam->followLookAt.y, rotated.y);
  tmp.z = VfAdd(cam->followLookAt.z, rotated.z);
  tmp.w = cam->followLookAt.w;
  /* eye += (tmp - eye) * blend; target += (followLookAt - target) * blend */
  cam->base.eye[0] = VfAdd(cam->base.eye[0], VfMul(VfSub(tmp.x, cam->base.eye[0]), blend));
  cam->base.eye[1] = VfAdd(cam->base.eye[1], VfMul(VfSub(tmp.y, cam->base.eye[1]), blend));
  cam->base.eye[2] = VfAdd(cam->base.eye[2], VfMul(VfSub(tmp.z, cam->base.eye[2]), blend));
  cam->base.eye[3] = VfAdd(cam->base.eye[3], VfMul(VfSub(tmp.w, cam->base.eye[3]), blend));
  cam->base.target[0] = VfAdd(cam->base.target[0], VfMul(VfSub(cam->followLookAt.x, cam->base.target[0]), blend));
  cam->base.target[1] = VfAdd(cam->base.target[1], VfMul(VfSub(cam->followLookAt.y, cam->base.target[1]), blend));
  cam->base.target[2] = VfAdd(cam->base.target[2], VfMul(VfSub(cam->followLookAt.z, cam->base.target[2]), blend));
  cam->base.target[3] = VfAdd(cam->base.target[3], VfMul(VfSub(cam->followLookAt.w, cam->base.target[3]), blend));
  GameFieldCameraAvoidWalls(cam);
}
