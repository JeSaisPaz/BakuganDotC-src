// bdc 0x088bc1d0 GameFieldCameraModeFollow
#include "bdc.h"

/* Step-follow mode of the field camera (embedded at `+0x20` of the field scene task,
   `GameFieldUpdate`; also run 10 times by `GameFieldCameraBeginBlendToFollow`). Does nothing
   without a target. With a quest camera (`questCam`) it only steps it
   (`GameFieldCameraUpdateQuestCam`) and fades the player near the eye
   (`GameFieldCameraFadeNearPlayer`). Otherwise it runs by `stepState`:
   - 0: `followLookAt` (saved first into `followGoal`) eases halfway towards the target position
     raised by `g_gameFieldCameraLookAtHeight`; `yaw` turns 30% of the wrapped gap towards the
     back of the target (heading + pi) and is wrapped to (-pi, pi]. The orbit radius eases 40% from
     the eye distance to `distance`, the elevation 20% towards `-pitch`. Target flag `0x100000` (in
     `motion`) spins `yawVel` down by 0.01 (floor -0.1), `0x200000` up by 0.01 (cap 0.1), else it
     decays by 25%; the orbit yaw is the eye's current azimuth plus `yawVel`. `pitchVel` grows by 0.1
     up to 1 and its square is the blend factor. The eye (saved first in `prevEye`) and look-at ease
     towards the orbit point rotated about the horizontal axis with `MathQuatFromAxisAngle` and
     towards `followLookAt`.
   - 1: like 0, but `followLookAt` eases by 0.35, `pitch` eases 20% towards 0.12217305 and the
     yaw step is `(1 - cos(pi)) / 2 * gap * 0.3` with gap the wrapped difference between the eye's
     azimuth and `yaw`. A step below 0.01 zeroes `yawVel` and advances `stepState`; one up to 0.04
     halves `yawVel` but keeps it at least 0.03 in magnitude; a larger one moves `yawVel` 20% of the
     wrapped `yawVel - step` gap, clamped to [-0.8, 0.8]. `prevEye` is not saved and the rotation
     quaternion is built inline.
   - 2: advances `stepState` unless target flag `0x1000000` is set, then runs
     `GameFieldCameraFollowStep`.
   States 0 and 1 end with `GameFieldCameraAvoidWalls`; other states do nothing. */

void GameFieldCameraModeFollow(GameFieldCamera *cam)

{
  ScePspFVector4 look;    /* sp+0x00 */
  ScePspFVector4 offset;  /* sp+0x10 */
  ScePspFVector4 axis;    /* sp+0x20 */
  ScePspFVector4 quatBuf; /* sp+0x30 */
  ScePspFVector4 tmp;
  ScePspFVector4 rot;
  ScePspFVector4 r;
  float *quat;
  float qx, qy, qz, qw;
  float dist;
  float radius;
  float yaw;
  float blend;
  float angle;
  float elev;
  float azimuth;
  float d;
  float v;
  float k;
  float step;
  float half;
  s32 state;

  if (cam->target == NULL) {
    return;
  }
  if (cam->questCam != NULL) {
    GameFieldCameraUpdateQuestCam(cam);
    GameFieldCameraFadeNearPlayer(cam->base.eye);
    return;
  }
  state = cam->stepState;
  if (state >= 2) {
    if (state < 3) {
      if ((((Actor *)cam->target)->motion & 0x1000000) == 0) {
        cam->stepState = cam->stepState + 1;
      }
      GameFieldCameraFollowStep(cam);
    }
    return;
  }
  if (state < 0) {
    return;
  }

  if (state <= 0) {
    look = *(ScePspFVector4 *)((Actor *)cam->target)->base.pos;
    look.y = look.y + g_gameFieldCameraLookAtHeight;
    cam->followGoal = cam->followLookAt;
    /* followLookAt += (look - followLookAt) * 0.5 */
    cam->followLookAt.x = cam->followLookAt.x + (look.x - cam->followLookAt.x) * 0.5f;
    cam->followLookAt.y = cam->followLookAt.y + (look.y - cam->followLookAt.y) * 0.5f;
    cam->followLookAt.z = cam->followLookAt.z + (look.z - cam->followLookAt.z) * 0.5f;
    cam->followLookAt.w = cam->followLookAt.w + (look.w - cam->followLookAt.w) * 0.5f;
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
    cam->yaw = cam->yaw + d * 0.3f;
    if (!(cam->yaw <= 3.1415927f)) {
      cam->yaw = cam->yaw - 6.2831855f;
    }
    else if (cam->yaw <= -3.1415927f) {
      cam->yaw = cam->yaw + 6.2831855f;
    }
    /* offset = eye - followLookAt (xyz, w = eye.w); dist = |offset.xyz| */
    offset.x = cam->base.eye[0] - cam->followLookAt.x;
    offset.y = cam->base.eye[1] - cam->followLookAt.y;
    offset.z = cam->base.eye[2] - cam->followLookAt.z;
    offset.w = cam->base.eye[3];
    dist = __builtin_sqrtf(offset.x * offset.x + offset.y * offset.y + offset.z * offset.z);
    radius = dist + (cam->distance - dist) * 0.4f;
    elev = -atan2f(offset.y, __builtin_sqrtf(offset.x * offset.x + offset.z * offset.z));
    angle = elev + (-cam->pitch - elev) * 0.2f;
    azimuth = atan2f(offset.z, offset.x);
    if ((((Actor *)cam->target)->motion & 0x100000) != 0) {
      v = cam->yawVel - 0.01f;
      if (v < -0.1f) {
        v = -0.1f;
      }
      cam->yawVel = v;
    }
    else {
      if ((((Actor *)cam->target)->motion & 0x200000) != 0) {
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
    yaw = azimuth + v;
    /* offset = (cos yaw, 0, sin yaw, 0) * radius (w unscaled); axis = (offset.z, offset.y, -offset.x, 0) */
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
    cam->prevEye = *(ScePspFVector4 *)cam->base.eye;
    /* axis = normalize(axis), each lane clamped to [-1, 1]; zero length gives 0 */
    d = axis.x * axis.x + axis.y * axis.y + axis.z * axis.z;
    k = VfRsq(d);
    if (d == 0.0f) {
      k = 0.0f;
    }
    axis.x = VfSat1(axis.x * k);
    axis.y = VfSat1(axis.y * k);
    axis.z = VfSat1(axis.z * k);
    axis.w = 0.0f;
    quat = MathQuatFromAxisAngle(angle, (float *)&quatBuf, (const float *)&axis);
  }
  else {
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
    cam->yaw = cam->yaw + d * 0.3f;
    if (!(cam->yaw <= 3.1415927f)) {
      cam->yaw = cam->yaw - 6.2831855f;
    }
    else if (cam->yaw <= -3.1415927f) {
      cam->yaw = cam->yaw + 6.2831855f;
    }
    cam->pitch = cam->pitch + (0.12217305f - cam->pitch) * 0.2f;
    /* offset = eye - followLookAt (xyz, w = eye.w); dist = |offset.xyz| */
    offset.x = cam->base.eye[0] - cam->followLookAt.x;
    offset.y = cam->base.eye[1] - cam->followLookAt.y;
    offset.z = cam->base.eye[2] - cam->followLookAt.z;
    offset.w = cam->base.eye[3];
    dist = __builtin_sqrtf(offset.x * offset.x + offset.y * offset.y + offset.z * offset.z);
    radius = dist + (cam->distance - dist) * 0.4f;
    elev = -atan2f(offset.y, __builtin_sqrtf(offset.x * offset.x + offset.z * offset.z));
    angle = elev + (-cam->pitch - elev) * 0.2f;
    azimuth = atan2f(offset.z, offset.x);
    d = azimuth - cam->yaw;
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
    step = (1.0f - __builtin_cosf(3.14159274f)) * 0.5f * d * 0.3f;
    if (__builtin_fabsf(step) < 0.01f) {
      cam->yawVel = 0.0f;
      cam->stepState = cam->stepState + 1;
    }
    else if (__builtin_fabsf(step) <= 0.04f) {
      cam->yawVel = cam->yawVel * 0.5f;
      if (cam->yawVel < 0.0f) {
        v = cam->yawVel;
        if (!(v <= -0.03f)) {
          v = -0.03f;
        }
        cam->yawVel = v;
      }
      else {
        v = cam->yawVel;
        if (v < 0.03f) {
          v = 0.03f;
        }
        cam->yawVel = v;
      }
    }
    else {
      d = cam->yawVel - step;
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
      cam->yawVel = cam->yawVel + d * 0.2f;
      if (cam->yawVel < 0.0f) {
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
    yaw = azimuth + cam->yawVel;
    /* offset = (cos yaw, 0, sin yaw, 0) * radius (w unscaled); axis = (offset.z, offset.y, -offset.x, 0) */
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
    /* axis = normalize(axis), each lane clamped to [-1, 1]; zero length gives 0 */
    d = axis.x * axis.x + axis.y * axis.y + axis.z * axis.z;
    k = VfRsq(d);
    if (d == 0.0f) {
      k = 0.0f;
    }
    axis.x = VfSat1(axis.x * k);
    axis.y = VfSat1(axis.y * k);
    axis.z = VfSat1(axis.z * k);
    axis.w = 0.0f;
    /* quatBuf = (axis * sin(angle / 2), cos(angle / 2)) */
    half = 0.318309873f * angle;
    quatBuf.w = VfCosQuarter(half);
    v = VfSinQuarter(half);
    quatBuf.x = axis.x * v;
    quatBuf.y = axis.y * v;
    quatBuf.z = axis.z * v;
    quat = (float *)&quatBuf;
  }
  /* rot = quat * (offset.xyz, 0) * conj(quat) */
  qx = quat[0];
  qy = quat[1];
  qz = quat[2];
  qw = quat[3];
  r.x = qx * 0.0f + qy * offset.z - qz * offset.y + qw * offset.x;
  r.y = -qx * offset.z + qy * 0.0f + qz * offset.x + qw * offset.y;
  r.z = qx * offset.y - qy * offset.x + qz * 0.0f + qw * offset.z;
  r.w = -qx * offset.x - qy * offset.y - qz * offset.z + qw * 0.0f;
  rot.x = r.x * qw + r.y * -qz - r.z * -qy + r.w * -qx;
  rot.y = -r.x * -qz + r.y * qw + r.z * -qx + r.w * -qy;
  rot.z = r.x * -qy - r.y * -qx + r.z * qw + r.w * -qz;
  rot.w = -r.x * -qx - r.y * -qy - r.z * -qz + r.w * qw;
  /* tmp = followLookAt + rot (xyz, w = followLookAt.w) */
  tmp.x = cam->followLookAt.x + rot.x;
  tmp.y = cam->followLookAt.y + rot.y;
  tmp.z = cam->followLookAt.z + rot.z;
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
