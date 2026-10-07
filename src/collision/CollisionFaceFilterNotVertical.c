// bdc 0x089e5108 CollisionFaceFilterNotVertical
#include "bdc.h"

/* Face filter (mode 4 of `CollisionSetFaceFilter`): accepts faces whose (part-rotated) normal has
   `|Y| > 0.2` (floors and ceilings, not walls). */

bool CollisionFaceFilterNotVertical(const CollisionFacePart *part, const u16 *face)
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
    return !(__builtin_fabsf(y) <= 0.2f);
}
