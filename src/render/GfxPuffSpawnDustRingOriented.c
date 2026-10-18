// bdc 0x088290b0 GfxPuffSpawnDustRingOriented
#include "bdc.h"

/* Like `GfxPuffSpawnDustRing` (16 dust puffs, kind 1, `GfxPuffCreate`) but the ring is turned
   by the quaternion that takes up onto the surface normal `normal` (axis (n.z, 0, -n.x), angle
   `acosf(n.y)`) and spun by `angle` (radians) about Y first. NULL `normal` uses `g_vecUp` and the
   identity rotation (bank C730, no spin); a vertical normal uses `g_quatIdentity` · spin. Each
   puff's velocity (`scaleX..Z`, +0x90) is the ring direction i·2π/16 times 1.0–1.1, its x scaled by
   `scaleX` and z by `scaleZ`, rotated; the puff is pushed 1.5 along the normal plus its velocity,
   `dir·0.5` (if non-NULL) is added to the velocity, `matrix` row 0 = normal·0.008, and a random
   size 2.2–4.7 is set. Used by `GfxEffectRunCommands`. */

void GfxPuffSpawnDustRingOriented(float angle, float scaleX, float scaleZ, const float *pos,
                                  const float *dir, const float *normal)
{
  s32 i;
  GfxPuff *puff;
  float size;
  float ringAngle;
  float speed;
  float rnd;
  float theta;
  float half;
  float lenSq;
  float k;
  float s;
  float c;
  s32 vertical;
  ScePspFVector4 quat;
  ScePspFVector4 spin;
  ScePspFVector4 tilt;
  ScePspFVector4 axis;
  ScePspFVector4 ringVel;
  ScePspFVector4 conj;
  ScePspFVector4 tmp;
  ScePspFVector4 rotated;

  if (normal == NULL) {
    normal = (const float *)&g_vecUp;
    /* bank C730: identity quaternion */
    quat.x = 0.0f;
    quat.y = 0.0f;
    quat.z = 0.0f;
    quat.w = 1.0f;
  } else {
    vertical = 0;
    if (normal[0] * normal[0] + normal[2] * normal[2] < 1e-05f) {
      if (!(normal[1] * normal[1] <= 0.0001f)) {
        vertical = 1;
      }
    }
    if (vertical != 0) {
      /* spin = Y-rotation by angle: (0, sin(angle/2), 0, cos(angle/2)) */
      half = VfMul(angle, 0.318309873f);
      spin.x = 0.0f;
      spin.y = VfSinQuarter(half);
      spin.z = 0.0f;
      spin.w = VfCosQuarter(half);
      /* quat = identity * spin */
      quat.x = VfDot4(g_quatIdentity.x, g_quatIdentity.y, g_quatIdentity.z, g_quatIdentity.w, spin.w, spin.z, -spin.y, spin.x);
      quat.y = VfDot4(-g_quatIdentity.x, g_quatIdentity.y, g_quatIdentity.z, g_quatIdentity.w, spin.z, spin.w, spin.x, spin.y);
      quat.z = VfDot4(g_quatIdentity.x, g_quatIdentity.y, g_quatIdentity.z, g_quatIdentity.w, spin.y, -spin.x, spin.w, spin.z);
      quat.w = VfDot4(-g_quatIdentity.x, g_quatIdentity.y, g_quatIdentity.z, g_quatIdentity.w, spin.x, -spin.y, -spin.z, spin.w);
    } else {
      axis.x = normal[2];
      axis.y = 0.0f;
      axis.z = -normal[0];
      /* axis = normalize(axis) (zero length: scale by 0), saturated to [-1,1] */
      lenSq = VfDot3(axis.x, axis.y, axis.z, axis.x, axis.y, axis.z);
      if (VfIsZero(lenSq)) {
        k = 0.0f;
      } else {
        k = VfRsq(lenSq);
      }
      axis.x = VfSat1(VfMul(axis.x, k));
      axis.y = VfSat1(VfMul(axis.y, k));
      axis.z = VfSat1(VfMul(axis.z, k));
      theta = acosf(normal[1]);
      /* tilt = (axis * sin(theta/2), cos(theta/2)) */
      half = VfMul(0.318309873f, theta);
      c = VfCosQuarter(half);
      s = VfSinQuarter(half);
      tilt.x = VfMul(axis.x, s);
      tilt.y = VfMul(axis.y, s);
      tilt.z = VfMul(axis.z, s);
      tilt.w = c;
      /* spin = Y-rotation by angle */
      half = VfMul(angle, 0.318309873f);
      spin.x = 0.0f;
      spin.y = VfSinQuarter(half);
      spin.z = 0.0f;
      spin.w = VfCosQuarter(half);
      /* quat = tilt * spin */
      quat.x = VfDot4(tilt.x, tilt.y, tilt.z, tilt.w, spin.w, spin.z, -spin.y, spin.x);
      quat.y = VfDot4(-tilt.x, tilt.y, tilt.z, tilt.w, spin.z, spin.w, spin.x, spin.y);
      quat.z = VfDot4(tilt.x, tilt.y, tilt.z, tilt.w, spin.y, -spin.x, spin.w, spin.z);
      quat.w = VfDot4(-tilt.x, tilt.y, tilt.z, tilt.w, spin.x, -spin.y, -spin.z, spin.w);
    }
  }
  i = 0;
  do {
    puff = GfxPuffCreate(1, pos);
    ringAngle = (float)i * 6.2831855f * 0.0625f;
    rnd = VfSub(PlatformRandFloat12(), 1.0f);
    speed = rnd * 0.1f + 1.0f;
    /* ringVel = (cos a, 0, sin a) * speed */
    ringVel.x = VfMul(VfCosQuarter(VfMul(ringAngle, 0.636619747f)), speed);
    ringVel.y = VfMul(0.0f, speed);
    ringVel.z = VfMul(VfSinQuarter(VfMul(ringAngle, 0.636619747f)), speed);
    ringVel.x = ringVel.x * scaleX;
    ringVel.z = ringVel.z * scaleZ;
    ringVel.w = 0.0f;
    /* velocity = quat * ringVel * conj(quat) */
    conj.x = -quat.x;
    conj.y = -quat.y;
    conj.z = -quat.z;
    conj.w = quat.w;
    tmp.x = VfDot4(quat.x, quat.y, quat.z, quat.w, ringVel.w, ringVel.z, -ringVel.y, ringVel.x);
    tmp.y = VfDot4(-quat.x, quat.y, quat.z, quat.w, ringVel.z, ringVel.w, ringVel.x, ringVel.y);
    tmp.z = VfDot4(quat.x, quat.y, quat.z, quat.w, ringVel.y, -ringVel.x, ringVel.w, ringVel.z);
    tmp.w = VfDot4(-quat.x, quat.y, quat.z, quat.w, ringVel.x, -ringVel.y, -ringVel.z, ringVel.w);
    rotated.x = VfDot4(tmp.x, tmp.y, tmp.z, tmp.w, conj.w, conj.z, -conj.y, conj.x);
    rotated.y = VfDot4(-tmp.x, tmp.y, tmp.z, tmp.w, conj.z, conj.w, conj.x, conj.y);
    rotated.z = VfDot4(tmp.x, tmp.y, tmp.z, tmp.w, conj.y, -conj.x, conj.w, conj.z);
    rotated.w = VfDot4(-tmp.x, tmp.y, tmp.z, tmp.w, conj.x, -conj.y, -conj.z, conj.w);
    puff->base.scaleX = rotated.x;
    puff->base.scaleY = rotated.y;
    puff->base.scaleZ = rotated.z;
    puff->base.angle = rotated.w;
    /* position += normal * 1.5; position += velocity (w kept) */
    puff->base.posX = VfAdd(puff->base.posX, VfMul(normal[0], 1.5f));
    puff->base.posY = VfAdd(puff->base.posY, VfMul(normal[1], 1.5f));
    puff->base.posZ = VfAdd(puff->base.posZ, VfMul(normal[2], 1.5f));
    puff->base.posX = VfAdd(puff->base.posX, puff->base.scaleX);
    puff->base.posY = VfAdd(puff->base.posY, puff->base.scaleY);
    puff->base.posZ = VfAdd(puff->base.posZ, puff->base.scaleZ);
    if (dir != NULL) {
      /* velocity += dir * 0.5 (w kept) */
      puff->base.scaleX = VfAdd(puff->base.scaleX, VfMul(dir[0], 0.5f));
      puff->base.scaleY = VfAdd(puff->base.scaleY, VfMul(dir[1], 0.5f));
      puff->base.scaleZ = VfAdd(puff->base.scaleZ, VfMul(dir[2], 0.5f));
    }
    /* matrix row 0 = (normal * 0.008, 0) */
    puff->base.matrix[0] = VfMul(normal[0], 0.008f);
    puff->base.matrix[1] = VfMul(normal[1], 0.008f);
    puff->base.matrix[2] = VfMul(normal[2], 0.008f);
    puff->base.matrix[3] = 0.0f;
    rnd = VfSub(PlatformRandFloat12(), 1.0f);
    size = rnd * 2.5f + 2.2f;
    puff->base.width = size;
    puff->base.height = size;
    puff->base.depth = size;
    puff->base.maybe_sizeW = 0.0f;
    i++;
  } while (i < 16);
}
