// bdc 0x089e8418 GfxEffectChainSolveBackward
#include "bdc.h"

/* Same constraint as `GfxEffectChainSolveForward` run from the tail to the head (used for chains
   anchored at both ends, flag `twoSided`): for each link i = count-1 .. 1, when the distance d from
   `points[i]` to `points[i-1]` is not <= 0 and not <= the link's maximum length L (`linkLengths[i]`,
   or `maxLinkLength` without the per-link array), `points[i-1]` is moved to `points[i]` plus the
   link direction scaled by L, and 0.03 * (d - L) along the direction is subtracted from
   `velocities[i]`. Only x, y, z are written; the original stores stale VFPU lanes into the `w`
   words. No return value. */
void GfxEffectChainSolveBackward(GfxEffectChain *chain)
{
    s32 i;

    for (i = chain->count - 1; i > 0; i--) {
        ScePspFVector4 *a = &chain->points[i - 1];
        ScePspFVector4 *b = &chain->points[i];
        float maxLen = (chain->linkLengths != NULL) ? chain->linkLengths[i] : chain->maxLinkLength;
        float dx = a->x - b->x;
        float dy = a->y - b->y;
        float dz = a->z - b->z;
        float dist = __builtin_sqrtf(dx * dx + dy * dy + dz * dz);

        if (!(dist <= 0.0f) && !(dist <= maxLen)) {
            float inv = 1.0f / dist;
            float pull = (dist - maxLen) * 0.03f * inv;
            float keep = inv * maxLen;
            ScePspFVector4 *v = &chain->velocities[i];
            float vx = v->x;
            float vy = v->y;
            float vz = v->z;

            a->x = b->x + dx * keep;
            a->y = b->y + dy * keep;
            a->z = b->z + dz * keep;
            v->x = vx - dx * pull;
            v->y = vy - dy * pull;
            v->z = vz - dz * pull;
        }
    }
}
