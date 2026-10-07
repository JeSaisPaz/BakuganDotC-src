// bdc 0x089e8040 GfxEffectChainUpdate
#include "bdc.h"

/* Per-frame update of an effect chain (nothing when `points` is NULL). Without physics the points
   shift one slot down the array (a trail: points[i] = points[i - 1] from the tail) and the head
   takes `*anchor` when there is one. With physics the head is pinned to `*anchor` (if any),
   velocities are added to the points (xyz), the link constraints are solved
   (`GfxEffectChainSolveForward`; two-sided chains first run `GfxEffectChainSolveBackward` and
   re-pin the head), then per point: velocity.xyz = ((point − prevPoint) × 0.1 + velocity) ×
   `stiffness`, velocity.y += `damping`, and prevPoint = point. The original also stored a stale
   VFPU lane (S030, never set in the loop) into each velocity's w; the lift leaves w unchanged. */

void GfxEffectChainUpdate(GfxEffectChain *chain)
{
  ScePspFVector4 *p;
  ScePspFVector4 *v;
  ScePspFVector4 *prev;
  ScePspFVector4 cur;
  float stiffness;
  float damping;
  float x;
  float y;
  float z;
  s32 i;

  if (chain->points == NULL) {
    return;
  }
  if (chain->physics == 0) {
    for (i = chain->count - 1; i > 0; i--) {
      chain->points[i] = chain->points[i - 1];
    }
    if (chain->anchor != NULL) {
      chain->points[0] = *chain->anchor;
    }
    return;
  }

  if (chain->anchor != NULL) {
    chain->points[0] = *chain->anchor;
  }
  for (i = 0; i < chain->count; i++) {
    p = &chain->points[i];
    v = &chain->velocities[i];
    p->x = p->x + v->x;
    p->y = p->y + v->y;
    p->z = p->z + v->z;
  }
  if (chain->twoSided) {
    GfxEffectChainSolveBackward(chain);
    if (chain->anchor != NULL) {
      chain->points[0] = *chain->anchor;
    }
    GfxEffectChainSolveForward(chain);
  } else {
    GfxEffectChainSolveForward(chain);
  }

  stiffness = chain->stiffness;
  damping = chain->damping;
  for (i = 0; i < chain->count; i++) {
    cur = chain->points[i];
    prev = &chain->prevPoints[i];
    v = &chain->velocities[i];
    x = (cur.x - prev->x) * 0.1f;
    y = (cur.y - prev->y) * 0.1f;
    z = (cur.z - prev->z) * 0.1f;
    x = (x + v->x) * stiffness;
    y = (y + v->y) * stiffness;
    z = (z + v->z) * stiffness;
    v->x = x;
    v->y = y + damping;
    v->z = z;
    chain->prevPoints[i] = cur;
  }
}
