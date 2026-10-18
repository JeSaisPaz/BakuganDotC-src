// bdc 0x088bd688 GameFieldCameraModeOrbit
#include "bdc.h"

/* Mode 7 (orbit) of the field camera (embedded at `+0x20` of the field scene task,
   `GameFieldUpdate`). Does nothing without a target. Saves `followLookAt` into `followGoal` and
   eases it halfway towards the target position raised by `g_gameFieldCameraLookAtHeight`. The
   orbit radius eases 40% from the current eye distance towards `distance`; the elevation eases 20%
   from the current one towards `orbitPitch - pitch`. Target flag `0x100000` (in `motion`) spins
   `yawVel` down by 0.01 (floor -0.1), `0x200000` up by 0.01 (cap 0.1), else it decays by 25%;
   `talkSideOffset` accumulates it and is the orbit yaw. `pitchVel` grows by 0.1 up to 1 and its
   square is the blend factor. The eye (saved first in `prevEye`) and look-at ease towards the
   rotated orbit point (`MathQuatFromAxisAngle`) and `followLookAt`. Then the path from the
   eased `prevEye` towards the orbit point at elevation `-orbitPitch` is ray-tested
   (`CollisionRaycastRay`, mask `0x2f800700`): on a hit closer than the look-at, `orbitPitch`
   drops by 0.05 while `pitch - orbitPitch` < 1.4207964 and `wallAvoidTimer` becomes 1. Without a
   hit a running `wallAvoidTimer` counts down; else, with `orbitPitch` < 0, a second probe 0.11 rad
   lower that also misses raises `orbitPitch` by 0.05, capped at 0. */

/* d += (to - d) * k on all four lanes (vsub.q, vscl.q, vadd.q). */
static void OrbitEase(ScePspFVector4 *d, const ScePspFVector4 *to, float k)
{
  d->x = VfAdd(d->x, VfMul(VfSub(to->x, d->x), k));
  d->y = VfAdd(d->y, VfMul(VfSub(to->y, d->y), k));
  d->z = VfAdd(d->z, VfMul(VfSub(to->z, d->z), k));
  d->w = VfAdd(d->w, VfMul(VfSub(to->w, d->w), k));
}

/* out = q * (v.xyz, 0) * conj(q) (two vqmul.q; the 0 is the bank's S730). */
static void OrbitQuatRotate(ScePspFVector4 *out, const float *q, const ScePspFVector4 *v)
{
  float px = v->x;
  float py = v->y;
  float pz = v->z;
  float pw = 0.0f;
  float cx = -q[0];
  float cy = -q[1];
  float cz = -q[2];
  float cw = q[3];
  float tx;
  float ty;
  float tz;
  float tw;

  tx = VfDot4(q[0], q[1], q[2], q[3], pw, pz, -py, px);
  ty = VfDot4(-q[0], q[1], q[2], q[3], pz, pw, px, py);
  tz = VfDot4(q[0], q[1], q[2], q[3], py, -px, pw, pz);
  tw = VfDot4(-q[0], q[1], q[2], q[3], px, -py, -pz, pw);
  out->x = VfDot4(tx, ty, tz, tw, cw, cz, -cy, cx);
  out->y = VfDot4(-tx, ty, tz, tw, cz, cw, cx, cy);
  out->z = VfDot4(tx, ty, tz, tw, cy, -cx, cw, cz);
  out->w = VfDot4(-tx, ty, tz, tw, cx, -cy, -cz, cw);
}

void GameFieldCameraModeOrbit(GameFieldCamera *cam)

{
  ScePspFVector4 look;
  ScePspFVector4 offset;
  ScePspFVector4 axis;
  ScePspFVector4 rot1;
  ScePspFVector4 origin;
  ScePspFVector4 rot2;
  ScePspFVector4 rot3;
  ScePspFVector4 turned;
  ScePspFVector4 point;
  ScePspFVector4 dir;
  ScePspFVector4 *eye;
  ScePspFVector4 *target;
  ScePspFVector4 *hit;
  float *quat;
  float dist;
  float radius;
  float yaw;
  float blend;
  float elev;
  float angle;
  float k;
  float v;
  float dx;
  float dy;
  float dz;
  float d1;
  float d2;

  if (cam->target == NULL) {
    return;
  }
  eye = (ScePspFVector4 *)cam->base.eye;
  target = (ScePspFVector4 *)cam->base.target;
  look = *(ScePspFVector4 *)((Actor *)cam->target)->base.pos;
  look.y = look.y + g_gameFieldCameraLookAtHeight;
  cam->followGoal = cam->followLookAt;
  OrbitEase(&cam->followLookAt, &look, 0.5f);
  /* offset = eye - followLookAt (xyz, w = eye.w); dist = |offset.xyz| */
  offset.x = VfSub(eye->x, cam->followLookAt.x);
  offset.y = VfSub(eye->y, cam->followLookAt.y);
  offset.z = VfSub(eye->z, cam->followLookAt.z);
  offset.w = eye->w;
  dist = VfSqrt(VfDot3(offset.x, offset.y, offset.z, offset.x, offset.y, offset.z));
  radius = dist + (cam->distance - dist) * 0.4f;
  elev = -atan2f(offset.y, __builtin_sqrtf(offset.x * offset.x + offset.z * offset.z));
  angle = elev + ((cam->orbitPitch - cam->pitch) - elev) * 0.2f;
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
  cam->talkSideOffset = cam->talkSideOffset + v;
  yaw = cam->talkSideOffset;
  /* vrot [C,0,S,0] of yaw * 2/pi, scaled by radius on xyz; w stays 0 */
  offset.x = VfMul(VfCosQuarter(VfMul(yaw, 0.636619747f)), radius);
  offset.y = VfMul(0.0f, radius);
  offset.z = VfMul(VfSinQuarter(VfMul(yaw, 0.636619747f)), radius);
  offset.w = 0.0f;
  /* axis = (offset.z, offset.y, -offset.x), w = 0 */
  axis.x = offset.z;
  axis.y = offset.y;
  axis.z = -offset.x;
  axis.w = 0.0f;
  blend = cam->pitchVel + 0.1f;
  if (!(blend <= 1.0f)) {
    blend = 1.0f;
  }
  cam->pitchVel = blend;
  blend = blend * blend;
  cam->prevEye = *eye;
  /* axis = normalize(axis), clamped to [-1, 1]; a zero-length axis scales by 0 (S713) */
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
  quat = MathQuatFromAxisAngle(angle, (float *)&rot1, (const float *)&axis);
  OrbitQuatRotate(&turned, quat, &offset);
  point.x = VfAdd(cam->followLookAt.x, turned.x);
  point.y = VfAdd(cam->followLookAt.y, turned.y);
  point.z = VfAdd(cam->followLookAt.z, turned.z);
  point.w = cam->followLookAt.w;
  OrbitEase(eye, &point, blend);
  OrbitEase(target, &cam->followLookAt, blend);
  origin = cam->prevEye;
  quat = MathQuatFromAxisAngle(elev - cam->orbitPitch, (float *)&rot2, (const float *)&axis);
  OrbitQuatRotate(&turned, quat, &offset);
  point.x = VfAdd(cam->followLookAt.x, turned.x);
  point.y = VfAdd(cam->followLookAt.y, turned.y);
  point.z = VfAdd(cam->followLookAt.z, turned.z);
  point.w = cam->followLookAt.w;
  OrbitEase(&origin, &point, blend);
  dir.x = VfSub(target->x, origin.x);
  dir.y = VfSub(target->y, origin.y);
  dir.z = VfSub(target->z, origin.z);
  dir.w = target->w;
  if (CollisionRaycastRay(0x2f800700, (const float *)&origin, (const float *)&dir) != NULL) {
    /* d1 = |origin - followLookAt|^2; d2 = |origin - hit point|^2 */
    dx = VfSub(origin.x, cam->followLookAt.x);
    dy = VfSub(origin.y, cam->followLookAt.y);
    dz = VfSub(origin.z, cam->followLookAt.z);
    d1 = VfDot3(dx, dy, dz, dx, dy, dz);
    hit = &g_collisionHitResult.point;
    dx = VfSub(origin.x, hit->x);
    dy = VfSub(origin.y, hit->y);
    dz = VfSub(origin.z, hit->z);
    d2 = VfDot3(dx, dy, dz, dx, dy, dz);
    if (d1 <= d2) {
      return;
    }
    if (cam->pitch - cam->orbitPitch < 1.4207964f) {
      cam->orbitPitch = cam->orbitPitch - 0.05f;
    }
    cam->wallAvoidTimer = 1;
    return;
  }
  if (cam->wallAvoidTimer != 0) {
    cam->wallAvoidTimer = cam->wallAvoidTimer - 1;
    return;
  }
  if (!(cam->orbitPitch < 0.0f)) {
    return;
  }
  angle = (elev - cam->orbitPitch) + -0.11f;
  origin = cam->prevEye;
  quat = MathQuatFromAxisAngle(angle, (float *)&rot3, (const float *)&axis);
  OrbitQuatRotate(&dir, quat, &offset);
  point.x = VfAdd(cam->followLookAt.x, dir.x);
  point.y = VfAdd(cam->followLookAt.y, dir.y);
  point.z = VfAdd(cam->followLookAt.z, dir.z);
  point.w = cam->followLookAt.w;
  OrbitEase(&origin, &point, blend);
  dir.x = VfSub(target->x, origin.x);
  dir.y = VfSub(target->y, origin.y);
  dir.z = VfSub(target->z, origin.z);
  dir.w = target->w;
  if (CollisionRaycastRay(0x2f800700, (const float *)&origin, (const float *)&dir) != NULL) {
    return;
  }
  v = cam->orbitPitch + 0.05f;
  cam->orbitPitch = v;
  if (!(v <= 0.0f)) {
    cam->orbitPitch = 0.0f;
  }
}
