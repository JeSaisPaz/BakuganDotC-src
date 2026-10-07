// bdc 0x08828d94 GfxPuffSpawnDustBurst
#include "bdc.h"

/* Spawns 3 dust puffs (kind 1, `GfxPuffCreate`) at `pos`, with small outward velocities
   (0.1–0.2, `scaleX..Z` +0x90) on a ring at angles i·2π/3 rotated by the quaternion that turns up
   onto the surface normal `normal` (axis (n.z, 0, -n.x), angle `acosf(n.y)`; NULL = `g_vecUp`
   and a vertical normal use the identity quaternion (0, 0, 0, 1)). Each puff is pushed 1.5 along
   the normal plus its velocity, `extraVel` (if non-NULL) is added to the velocity, `matrix` row 0 =
   (normal·0.008, 0), and a random size 2–3.5 is set. Used by `GfxEffectRunCommands`. */

void GfxPuffSpawnDustBurst(const float *pos, const float *extraVel, const float *normal)
{
  s32 i;
  GfxPuff *puff;
  float size;
  float angle;
  float speed;
  float theta;
  float half;
  float k;
  float len2;
  float s;
  float c;
  s32 vertical;
  ScePspFVector4 q;
  ScePspFVector4 axis;
  ScePspFVector4 v;
  ScePspFVector4 t;
  ScePspFVector4 r;

  if (normal == NULL) {
    normal = (const float *)&g_vecUp;
    q.x = 0.0f;
    q.y = 0.0f;
    q.z = 0.0f;
    q.w = 1.0f;
  } else {
    vertical = 0;
    if (normal[0] * normal[0] + normal[2] * normal[2] < 1e-05f) {
      if (!(normal[1] * normal[1] <= 0.0001f)) {
        vertical = 1;
      }
    }
    if (vertical != 0) {
      q.x = 0.0f;
      q.y = 0.0f;
      q.z = 0.0f;
      q.w = 1.0f;
    } else {
      axis.x = normal[2];
      axis.y = 0.0f;
      axis.z = -normal[0];
      /* axis = normalize(axis) (zero length: scale by 0), saturated to [-1,1] */
      len2 = axis.x * axis.x + axis.y * axis.y + axis.z * axis.z;
      k = VfRsq(len2);
      if (len2 == 0.0f) {
        k = 0.0f;
      }
      axis.x = VfSat1(axis.x * k);
      axis.y = VfSat1(axis.y * k);
      axis.z = VfSat1(axis.z * k);
      theta = acosf(normal[1]);
      /* quat = (axis * sin(theta/2), cos(theta/2)) */
      half = 0.318309873f * theta;
      c = VfCosQuarter(half);
      s = VfSinQuarter(half);
      q.x = axis.x * s;
      q.y = axis.y * s;
      q.z = axis.z * s;
      q.w = c;
    }
  }
  i = 0;
  do {
    puff = GfxPuffCreate(1, pos);
    angle = (float)i * 6.28318548f * 0.33334f;
    speed = (PlatformRandFloat12() - 1.0f) * 0.1f + 0.1f;
    /* ring velocity (cos a, 0, sin a)·speed, w = 0 */
    v.x = __builtin_cosf(angle) * speed;
    v.y = 0.0f * speed;
    v.z = __builtin_sinf(angle) * speed;
    v.w = 0.0f;
    /* velocity = q ⊗ v ⊗ conj(q) */
    t.x = q.x * v.w + q.y * v.z - q.z * v.y + q.w * v.x;
    t.y = -q.x * v.z + q.y * v.w + q.z * v.x + q.w * v.y;
    t.z = q.x * v.y - q.y * v.x + q.z * v.w + q.w * v.z;
    t.w = -q.x * v.x - q.y * v.y - q.z * v.z + q.w * v.w;
    r.x = t.x * q.w + t.y * -q.z - t.z * -q.y + t.w * -q.x;
    r.y = -t.x * -q.z + t.y * q.w + t.z * -q.x + t.w * -q.y;
    r.z = t.x * -q.y - t.y * -q.x + t.z * q.w + t.w * -q.z;
    r.w = -t.x * -q.x - t.y * -q.y - t.z * -q.z + t.w * q.w;
    puff->base.scaleX = r.x;
    puff->base.scaleY = r.y;
    puff->base.scaleZ = r.z;
    puff->base.angle = r.w;
    /* position += normal * 1.5; position += velocity */
    puff->base.posX = puff->base.posX + normal[0] * 1.5f;
    puff->base.posY = puff->base.posY + normal[1] * 1.5f;
    puff->base.posZ = puff->base.posZ + normal[2] * 1.5f;
    puff->base.posX = puff->base.posX + puff->base.scaleX;
    puff->base.posY = puff->base.posY + puff->base.scaleY;
    puff->base.posZ = puff->base.posZ + puff->base.scaleZ;
    if (extraVel != NULL) {
      puff->base.scaleX = puff->base.scaleX + extraVel[0];
      puff->base.scaleY = puff->base.scaleY + extraVel[1];
      puff->base.scaleZ = puff->base.scaleZ + extraVel[2];
    }
    /* matrix row 0 = (normal * 0.008, 0) */
    puff->base.matrix[0] = normal[0] * 0.008f;
    puff->base.matrix[1] = normal[1] * 0.008f;
    puff->base.matrix[2] = normal[2] * 0.008f;
    puff->base.matrix[3] = 0.0f;
    size = (PlatformRandFloat12() - 1.0f) * 1.5f + 2.0f;
    puff->base.width = size;
    puff->base.height = size;
    puff->base.depth = size;
    puff->base.maybe_sizeW = 0.0f;
    i++;
  } while (i < 3);
}
