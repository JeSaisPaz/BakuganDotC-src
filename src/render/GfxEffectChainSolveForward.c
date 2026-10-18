// bdc 0x089e8208 GfxEffectChainSolveForward
#include "bdc.h"

/* Link constraint pass from the head to the tail: for each link i (points[i] → points[i + 1]) whose
   length is above its maximum (`linkLengths[i]`, or the uniform `maxLinkLength` when there is no
   per-link array), points[i + 1] is moved to points[i] + dir × max / len and 0.03 × (len − max) ×
   dir / len is subtracted from velocities[i] (dir = points[i + 1] − points[i], xyz only). Links of
   length <= 0 are skipped. The w words of the stored quads are left out: the original stores
   stale VFPU lanes there. No return value. */

void GfxEffectChainSolveForward(GfxEffectChain *chain)
{
  s32 i;

  for (i = 0; i < chain->count - 1; i++) {
    ScePspFVector4 *a = &chain->points[i];
    ScePspFVector4 *b = &chain->points[i + 1];
    float maxLen = (chain->linkLengths != NULL) ? chain->linkLengths[i] : chain->maxLinkLength;
    float dx = VfSub(b->x, a->x);
    float dy = VfSub(b->y, a->y);
    float dz = VfSub(b->z, a->z);
    float len = VfSqrt(VfDot3(dx, dy, dz, dx, dy, dz));

    if (!(len <= 0.0f) && !(len <= maxLen)) {
      float inv = VfRcp(len);
      float pull = VfMul(VfMul(VfSub(len, maxLen), 0.03f), inv);
      float keep = VfMul(inv, maxLen);
      ScePspFVector4 *v = &chain->velocities[i];
      float vx = v->x;
      float vy = v->y;
      float vz = v->z;

      b->x = VfAdd(a->x, VfMul(dx, keep));
      b->y = VfAdd(a->y, VfMul(dy, keep));
      b->z = VfAdd(a->z, VfMul(dz, keep));
      v->x = VfSub(vx, VfMul(dx, pull));
      v->y = VfSub(vy, VfMul(dy, pull));
      v->z = VfSub(vz, VfMul(dz, pull));
    }
  }
}
