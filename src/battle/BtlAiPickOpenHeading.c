// bdc 0x0888f4d0 BtlAiPickOpenHeading
#include "bdc.h"

/* Picks a free heading around the owner of `BtlAi`: shuffles the eight angles
   `i x 45 deg` (eight random swaps with `CoreRandNext``(8)`), then for each casts a ray from
   the owner position to the direction vector `(cos, 0, sin)(angle) x *range`
   (`BtlAiRaycastBlocked`, clear distance out; an unblocked ray counts as `*range` + the owner's
   body radius). It keeps the largest clear distance seen (ties count) and stops at the first ray whose clear distance reaches `*range` + body radius.
   When at least one ray matched or improved the best, writes a heading to `*outAngle` and the best
   clear distance to `*range`; otherwise both are left unchanged. */
void BtlAiPickOpenHeading(BtlAi *self, float *outAngle, float *range)
{
    /* stack order matters for the quirk below: clearDist sits just before angles */
    struct {
        float clearDist;
        float angles[8];
    } rays;
    float ownerPos[4] __attribute__((aligned(16)));
    float dir[4] __attribute__((aligned(16)));
    float from[4] __attribute__((aligned(16)));
    float to[4] __attribute__((aligned(16)));
    float best;
    int improved;
    int i;
    int k;
    u32 a;
    u32 b;
    float tmp;

    for (i = 0; i < 4; i++) {
        ownerPos[i] = self->owner->base.pos[i];
    }
    best = 0.0f;
    improved = 0;
    for (i = 0; i < 8; i++) {
        rays.angles[i] = (float)i * 0.125f * 6.28318548f;
    }
    for (i = 0; i < 8; i++) {
        a = CoreRandNext(8) & 0xff;
        b = CoreRandNext(8) & 0xff;
        if (a != b) {
            tmp = rays.angles[a];
            rays.angles[a] = rays.angles[b];
            rays.angles[b] = tmp;
        }
    }
    for (i = 0; i < 8; i++) {
        rays.clearDist = *range + self->owner->combat.stats->bodyRadius;
        /* dir = (cos, 0, sin)(angle) scaled by *range; dir.w = 0 */
        dir[0] = __builtin_cosf(rays.angles[i]) * *range;
        dir[1] = 0.0f * *range;
        dir[2] = __builtin_sinf(rays.angles[i]) * *range;
        dir[3] = 0.0f;
        for (k = 0; k < 4; k++) {
            from[k] = ownerPos[k];
            to[k] = dir[k];
        }
        if (BtlAiRaycastBlocked(self, from, to, &rays.clearDist) == 0) {
            rays.clearDist = *range + self->owner->combat.stats->bodyRadius;
        }
        if (rays.clearDist < best) {
            continue;
        }
        best = rays.clearDist;
        improved++;
        if (*range + self->owner->combat.stats->bodyRadius <= best) {
            break;
        }
    }
    if (improved > 0) {
        /* quirk: the binary reads sp+0x20 + improved*4, i.e. angles[improved - 1], not the
           angle of the best ray */
        *outAngle = rays.angles[improved - 1];
        *range = best;
    }
}
