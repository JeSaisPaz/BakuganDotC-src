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
    behindX = __builtin_cosf(yaw) * -8.0f;
    behindZ = __builtin_sinf(yaw) * -8.0f;
    look.x = look.x + behindX;
    look.z = look.z + behindZ;
    look.y = look.y + g_gameFieldCameraHighLookAtHeight;
    cam->followGoal = cam->followLookAt;
    /* followLookAt += (look - followLookAt) * 0.35 */
    cam->followLookAt.x = cam->followLookAt.x + (look.x - cam->followLookAt.x) * 0.35f;
    cam->followLookAt.y = cam->followLookAt.y + (look.y - cam->followLookAt.y) * 0.35f;
    cam->followLookAt.z = cam->followLookAt.z + (look.z - cam->followLookAt.z) * 0.35f;
    cam->followLookAt.w = cam->followLookAt.w + (look.w - cam->followLookAt.w) * 0.35f;
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
    offset.x = cam->base.eye[0] - cam->followLookAt.x;
    offset.y = cam->base.eye[1] - cam->followLookAt.y;
    offset.z = cam->base.eye[2] - cam->followLookAt.z;
    offset.w = cam->base.eye[3];
    dist = __builtin_sqrtf(offset.x * offset.x + offset.y * offset.y + offset.z * offset.z);
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
    offset.x = __builtin_cosf(yaw) * radius;
    offset.y = 0.0f * radius;
    offset.z = __builtin_sinf(yaw) * radius;
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
    k = axis.x * axis.x + axis.y * axis.y + axis.z * axis.z;
    if (k == 0.0f) {
      k = 0.0f;
    }
    else {
      k = VfRsq(k);
    }
    axis.x = VfSat1(axis.x * k);
    axis.y = VfSat1(axis.y * k);
    axis.z = VfSat1(axis.z * k);
    axis.w = 0.0f;
    /* rot = (axis * sin(angle/2), cos(angle/2)): angle / pi in quarter turns */
    s = angle * 0.318309873f;
    rot.w = VfCosQuarter(s);
    s = VfSinQuarter(s);
    rot.x = axis.x * s;
    rot.y = axis.y * s;
    rot.z = axis.z * s;
    /* rotated = rot * (offset.xyz, 0) * conj(rot) */
    tmp.x = offset.x;
    tmp.y = offset.y;
    tmp.z = offset.z;
    tmp.w = 0.0f;
    mid.x = rot.x * tmp.w + rot.y * tmp.z - rot.z * tmp.y + rot.w * tmp.x;
    mid.y = -rot.x * tmp.z + rot.y * tmp.w + rot.z * tmp.x + rot.w * tmp.y;
    mid.z = rot.x * tmp.y - rot.y * tmp.x + rot.z * tmp.w + rot.w * tmp.z;
    mid.w = -rot.x * tmp.x - rot.y * tmp.y - rot.z * tmp.z + rot.w * tmp.w;
    rotated.x = mid.x * rot.w + mid.y * -rot.z - mid.z * -rot.y + mid.w * -rot.x;
    rotated.y = -mid.x * -rot.z + mid.y * rot.w + mid.z * -rot.x + mid.w * -rot.y;
    rotated.z = mid.x * -rot.y - mid.y * -rot.x + mid.z * rot.w + mid.w * -rot.z;
    rotated.w = -mid.x * -rot.x - mid.y * -rot.y - mid.z * -rot.z + mid.w * rot.w;
    /* tmp = followLookAt + rotated (xyz, w = followLookAt.w) */
    tmp.x = cam->followLookAt.x + rotated.x;
    tmp.y = cam->followLookAt.y + rotated.y;
    tmp.z = cam->followLookAt.z + rotated.z;
    tmp.w = cam->followLookAt.w;
    /* eye += (tmp - eye) * blend; target += (followLookAt - target) * blend */
    cam->base.eye[0] = cam->base.eye[0] + (tmp.x - cam->base.eye[0]) * blend;
    cam->base.eye[1] = cam->base.eye[1] + (tmp.y - cam->base.eye[1]) * blend;
    cam->base.eye[2] = cam->base.eye[2] + (tmp.z - cam->base.eye[2]) * blend;
    cam->base.eye[3] = cam->base.eye[3] + (tmp.w - cam->base.eye[3]) * blend;
    cam->base.target[0] = cam->base.target[0] + (cam->followLookAt.x - cam->base.target[0]) * blend;
    cam->base.target[1] = cam->base.target[1] + (cam->followLookAt.y - cam->base.target[1]) * blend;
    cam->base.target[2] = cam->base.target[2] + (cam->followLookAt.z - cam->base.target[2]) * blend;
    cam->base.target[3] = cam->base.target[3] + (cam->followLookAt.w - cam->base.target[3]) * blend;
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
  cam->followLookAt.x = cam->followLookAt.x + (look.x - cam->followLookAt.x) * 0.35f;
  cam->followLookAt.y = cam->followLookAt.y + (look.y - cam->followLookAt.y) * 0.35f;
  cam->followLookAt.z = cam->followLookAt.z + (look.z - cam->followLookAt.z) * 0.35f;
  cam->followLookAt.w = cam->followLookAt.w + (look.w - cam->followLookAt.w) * 0.35f;
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
  offset.x = cam->base.eye[0] - cam->followLookAt.x;
  offset.y = cam->base.eye[1] - cam->followLookAt.y;
  offset.z = cam->base.eye[2] - cam->followLookAt.z;
  offset.w = cam->base.eye[3];
  dist = __builtin_sqrtf(offset.x * offset.x + offset.y * offset.y + offset.z * offset.z);
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
  swing = (1.0f - __builtin_cosf(3.1415927f)) * 0.5f * d * 0.3f;
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
  offset.x = __builtin_cosf(yaw) * radius;
  offset.y = 0.0f * radius;
  offset.z = __builtin_sinf(yaw) * radius;
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
  k = axis.x * axis.x + axis.y * axis.y + axis.z * axis.z;
  if (k == 0.0f) {
    k = 0.0f;
  }
  else {
    k = VfRsq(k);
  }
  axis.x = VfSat1(axis.x * k);
  axis.y = VfSat1(axis.y * k);
  axis.z = VfSat1(axis.z * k);
  axis.w = 0.0f;
  quat = MathQuatFromAxisAngle(angle, (float *)&rot, (const float *)&axis);
  /* rotated = quat * (offset.xyz, 0) * conj(quat) */
  tmp.x = offset.x;
  tmp.y = offset.y;
  tmp.z = offset.z;
  tmp.w = 0.0f;
  mid.x = quat[0] * tmp.w + quat[1] * tmp.z - quat[2] * tmp.y + quat[3] * tmp.x;
  mid.y = -quat[0] * tmp.z + quat[1] * tmp.w + quat[2] * tmp.x + quat[3] * tmp.y;
  mid.z = quat[0] * tmp.y - quat[1] * tmp.x + quat[2] * tmp.w + quat[3] * tmp.z;
  mid.w = -quat[0] * tmp.x - quat[1] * tmp.y - quat[2] * tmp.z + quat[3] * tmp.w;
  rotated.x = mid.x * quat[3] + mid.y * -quat[2] - mid.z * -quat[1] + mid.w * -quat[0];
  rotated.y = -mid.x * -quat[2] + mid.y * quat[3] + mid.z * -quat[0] + mid.w * -quat[1];
  rotated.z = mid.x * -quat[1] - mid.y * -quat[0] + mid.z * quat[3] + mid.w * -quat[2];
  rotated.w = -mid.x * -quat[0] - mid.y * -quat[1] - mid.z * -quat[2] + mid.w * quat[3];
  /* tmp = followLookAt + rotated (xyz, w = followLookAt.w) */
  tmp.x = cam->followLookAt.x + rotated.x;
  tmp.y = cam->followLookAt.y + rotated.y;
  tmp.z = cam->followLookAt.z + rotated.z;
  tmp.w = cam->followLookAt.w;
  /* eye += (tmp - eye) * blend; target += (followLookAt - target) * blend */
  cam->base.eye[0] = cam->base.eye[0] + (tmp.x - cam->base.eye[0]) * blend;
  cam->base.eye[1] = cam->base.eye[1] + (tmp.y - cam->base.eye[1]) * blend;
  cam->base.eye[2] = cam->base.eye[2] + (tmp.z - cam->base.eye[2]) * blend;
  cam->base.eye[3] = cam->base.eye[3] + (tmp.w - cam->base.eye[3]) * blend;
  cam->base.target[0] = cam->base.target[0] + (cam->followLookAt.x - cam->base.target[0]) * blend;
  cam->base.target[1] = cam->base.target[1] + (cam->followLookAt.y - cam->base.target[1]) * blend;
  cam->base.target[2] = cam->base.target[2] + (cam->followLookAt.z - cam->base.target[2]) * blend;
  cam->base.target[3] = cam->base.target[3] + (cam->followLookAt.w - cam->base.target[3]) * blend;
  GameFieldCameraAvoidWalls(cam);
}
