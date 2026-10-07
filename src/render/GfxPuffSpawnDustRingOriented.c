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
      half = angle * 0.318309873f;
      spin.x = 0.0f;
      spin.y = VfSinQuarter(half);
      spin.z = 0.0f;
      spin.w = VfCosQuarter(half);
      /* quat = identity * spin */
      quat.x = g_quatIdentity.x * spin.w + g_quatIdentity.y * spin.z -
               g_quatIdentity.z * spin.y + g_quatIdentity.w * spin.x;
      quat.y = -g_quatIdentity.x * spin.z + g_quatIdentity.y * spin.w +
               g_quatIdentity.z * spin.x + g_quatIdentity.w * spin.y;
      quat.z = g_quatIdentity.x * spin.y - g_quatIdentity.y * spin.x +
               g_quatIdentity.z * spin.w + g_quatIdentity.w * spin.z;
      quat.w = -g_quatIdentity.x * spin.x - g_quatIdentity.y * spin.y -
               g_quatIdentity.z * spin.z + g_quatIdentity.w * spin.w;
    } else {
      axis.x = normal[2];
      axis.y = 0.0f;
      axis.z = -normal[0];
      /* axis = normalize(axis) (zero length: scale by 0), saturated to [-1,1] */
      lenSq = axis.x * axis.x + axis.y * axis.y + axis.z * axis.z;
      if (lenSq == 0.0f) {
        k = 0.0f;
      } else {
        k = VfRsq(lenSq);
      }
      axis.x = VfSat1(axis.x * k);
      axis.y = VfSat1(axis.y * k);
      axis.z = VfSat1(axis.z * k);
      theta = acosf(normal[1]);
      /* tilt = (axis * sin(theta/2), cos(theta/2)) */
      half = 0.318309873f * theta;
      c = VfCosQuarter(half);
      s = VfSinQuarter(half);
      tilt.x = axis.x * s;
      tilt.y = axis.y * s;
      tilt.z = axis.z * s;
      tilt.w = c;
      /* spin = Y-rotation by angle */
      half = angle * 0.318309873f;
      spin.x = 0.0f;
      spin.y = VfSinQuarter(half);
      spin.z = 0.0f;
      spin.w = VfCosQuarter(half);
      /* quat = tilt * spin */
      quat.x = tilt.x * spin.w + tilt.y * spin.z - tilt.z * spin.y + tilt.w * spin.x;
      quat.y = -tilt.x * spin.z + tilt.y * spin.w + tilt.z * spin.x + tilt.w * spin.y;
      quat.z = tilt.x * spin.y - tilt.y * spin.x + tilt.z * spin.w + tilt.w * spin.z;
      quat.w = -tilt.x * spin.x - tilt.y * spin.y - tilt.z * spin.z + tilt.w * spin.w;
    }
  }
  i = 0;
  do {
    puff = GfxPuffCreate(1, pos);
    ringAngle = (float)i * 6.2831855f * 0.0625f;
    rnd = PlatformRandFloat12() - 1.0f;
    speed = rnd * 0.1f + 1.0f;
    /* ringVel = (cos a, 0, sin a) * speed */
    ringVel.x = __builtin_cosf(ringAngle) * speed;
    ringVel.y = 0.0f * speed;
    ringVel.z = __builtin_sinf(ringAngle) * speed;
    ringVel.x = ringVel.x * scaleX;
    ringVel.z = ringVel.z * scaleZ;
    ringVel.w = 0.0f;
    /* velocity = quat * ringVel * conj(quat) */
    conj.x = -quat.x;
    conj.y = -quat.y;
    conj.z = -quat.z;
    conj.w = quat.w;
    tmp.x = quat.x * ringVel.w + quat.y * ringVel.z - quat.z * ringVel.y + quat.w * ringVel.x;
    tmp.y = -quat.x * ringVel.z + quat.y * ringVel.w + quat.z * ringVel.x + quat.w * ringVel.y;
    tmp.z = quat.x * ringVel.y - quat.y * ringVel.x + quat.z * ringVel.w + quat.w * ringVel.z;
    tmp.w = -quat.x * ringVel.x - quat.y * ringVel.y - quat.z * ringVel.z + quat.w * ringVel.w;
    rotated.x = tmp.x * conj.w + tmp.y * conj.z - tmp.z * conj.y + tmp.w * conj.x;
    rotated.y = -tmp.x * conj.z + tmp.y * conj.w + tmp.z * conj.x + tmp.w * conj.y;
    rotated.z = tmp.x * conj.y - tmp.y * conj.x + tmp.z * conj.w + tmp.w * conj.z;
    rotated.w = -tmp.x * conj.x - tmp.y * conj.y - tmp.z * conj.z + tmp.w * conj.w;
    puff->base.scaleX = rotated.x;
    puff->base.scaleY = rotated.y;
    puff->base.scaleZ = rotated.z;
    puff->base.angle = rotated.w;
    /* position += normal * 1.5; position += velocity (w kept) */
    puff->base.posX = puff->base.posX + normal[0] * 1.5f;
    puff->base.posY = puff->base.posY + normal[1] * 1.5f;
    puff->base.posZ = puff->base.posZ + normal[2] * 1.5f;
    puff->base.posX = puff->base.posX + puff->base.scaleX;
    puff->base.posY = puff->base.posY + puff->base.scaleY;
    puff->base.posZ = puff->base.posZ + puff->base.scaleZ;
    if (dir != NULL) {
      /* velocity += dir * 0.5 (w kept) */
      puff->base.scaleX = puff->base.scaleX + dir[0] * 0.5f;
      puff->base.scaleY = puff->base.scaleY + dir[1] * 0.5f;
      puff->base.scaleZ = puff->base.scaleZ + dir[2] * 0.5f;
    }
    /* matrix row 0 = (normal * 0.008, 0) */
    puff->base.matrix[0] = normal[0] * 0.008f;
    puff->base.matrix[1] = normal[1] * 0.008f;
    puff->base.matrix[2] = normal[2] * 0.008f;
    puff->base.matrix[3] = 0.0f;
    rnd = PlatformRandFloat12() - 1.0f;
    size = rnd * 2.5f + 2.2f;
    puff->base.width = size;
    puff->base.height = size;
    puff->base.depth = size;
    puff->base.maybe_sizeW = 0.0f;
    i++;
  } while (i < 16);
}
