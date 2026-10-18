// bdc 0x089e6d88 CollisionPhysBoxCentroid
#include "bdc.h"

/* Writes the centroid of the eight corner positions (sum × 0.125, `w` = 0) to `out`. */
void CollisionPhysBoxCentroid(CollisionPhysBox *self, ScePspFVector4 *out)
{
    ScePspFVector4 *pos = self->pos;
    float x = pos[0].x;
    float y = pos[0].y;
    float z = pos[0].z;
    s32 i;

    for (i = 1; i < 8; i++) {
        x = VfAdd(x, pos[i].x);
        y = VfAdd(y, pos[i].y);
        z = VfAdd(z, pos[i].z);
    }
    out->x = VfMul(x, 0.125f);
    out->y = VfMul(y, 0.125f);
    out->z = VfMul(z, 0.125f);
    out->w = 0.0f;
}
