// bdc 0x089e4f3c CollisionFaceFilterWall
#include "bdc.h"

/* Face filter (mode 1 of `CollisionSetFaceFilter`): accepts faces whose plane normal Y (rotated
   by `part->rotation` when present) is below 0.9, i.e. everything that is not a flat
   floor. */

bool CollisionFaceFilterWall(const CollisionFacePart *part, const u16 *face)
{
    const ScePspFVector4 *n = &part->normals[face[3]];
    float y;

    if (part->rotation != NULL) {
        const float *m = part->rotation;

        /* y = lane 1 of `vtfm3.t C000, E100, C200`: the part matrix rows (at 0x0, 0x10, 0x20) loaded
           as columns, so y = Σ_k n[k] · m[4k + 1] */
        y = m[1] * n->x + m[5] * n->y + m[9] * n->z;
    } else {
        y = n->y;
    }
    return y < 0.9f;
}
