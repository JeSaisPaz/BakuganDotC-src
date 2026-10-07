// bdc 0x089e6dfc CollisionPhysBoxRebuildShape
#include "bdc.h"

/* Re-squares the deformed box: derives its three axes from the eight corner positions (sum of one
   face's four corners minus the opposite face's), normalises them with `1/sqrt` (a zero-length axis
   stays zero; each lane clamped to [-1, 1]) and stores x/y/z in `axes[0..2]`, scales them by
   `halfExtents.x/.y/.z` and rebuilds the eight corners as `centre ± ax ± ay ± az`; the corners keep
   their own `w`. */

void CollisionPhysBoxRebuildShape(CollisionPhysBox *self, const ScePspFVector4 *centre)
{
    ScePspFVector4 a0;
    ScePspFVector4 a1;
    ScePspFVector4 a2;
    ScePspFVector4 lo;
    ScePspFVector4 hi;
    ScePspFVector4 t;
    ScePspFVector4 c[8];
    ScePspFVector4 nc[8];
    float d;
    float k;
    s32 i;

    for (i = 0; i < 8; i++) {
        c[i] = self->pos[i];
    }

    /* axis 0 = (c1 + c2 + c5 + c6) - (c0 + c3 + c4 + c7) */
    a0.x = c[1].x + c[2].x + c[5].x + c[6].x - c[0].x - c[3].x - c[4].x - c[7].x;
    a0.y = c[1].y + c[2].y + c[5].y + c[6].y - c[0].y - c[3].y - c[4].y - c[7].y;
    a0.z = c[1].z + c[2].z + c[5].z + c[6].z - c[0].z - c[3].z - c[4].z - c[7].z;
    d = a0.x * a0.x + a0.y * a0.y + a0.z * a0.z;
    k = (d == 0.0f) ? 0.0f : VfRsq(d);
    a0.x = VfSat1(a0.x * k);
    a0.y = VfSat1(a0.y * k);
    a0.z = VfSat1(a0.z * k);
    self->axes[0].x = a0.x;
    self->axes[0].y = a0.y;
    self->axes[0].z = a0.z;
    k = self->halfExtents.x;
    a0.x = a0.x * k;
    a0.y = a0.y * k;
    a0.z = a0.z * k;

    /* axis 1 = (c2 + c3 + c6 + c7) - (c0 + c1 + c4 + c5) */
    a1.x = c[2].x + c[3].x + c[6].x + c[7].x - c[0].x - c[1].x - c[4].x - c[5].x;
    a1.y = c[2].y + c[3].y + c[6].y + c[7].y - c[0].y - c[1].y - c[4].y - c[5].y;
    a1.z = c[2].z + c[3].z + c[6].z + c[7].z - c[0].z - c[1].z - c[4].z - c[5].z;
    d = a1.x * a1.x + a1.y * a1.y + a1.z * a1.z;
    k = (d == 0.0f) ? 0.0f : VfRsq(d);
    a1.x = VfSat1(a1.x * k);
    a1.y = VfSat1(a1.y * k);
    a1.z = VfSat1(a1.z * k);
    self->axes[1].x = a1.x;
    self->axes[1].y = a1.y;
    self->axes[1].z = a1.z;
    k = self->halfExtents.y;
    a1.x = a1.x * k;
    a1.y = a1.y * k;
    a1.z = a1.z * k;

    /* axis 2 = (c4 + c5 + c6 + c7) - (c0 + c1 + c2 + c3) */
    a2.x = c[4].x + c[5].x + c[6].x + c[7].x - c[0].x - c[1].x - c[2].x - c[3].x;
    a2.y = c[4].y + c[5].y + c[6].y + c[7].y - c[0].y - c[1].y - c[2].y - c[3].y;
    a2.z = c[4].z + c[5].z + c[6].z + c[7].z - c[0].z - c[1].z - c[2].z - c[3].z;
    d = a2.x * a2.x + a2.y * a2.y + a2.z * a2.z;
    k = (d == 0.0f) ? 0.0f : VfRsq(d);
    a2.x = VfSat1(a2.x * k);
    a2.y = VfSat1(a2.y * k);
    a2.z = VfSat1(a2.z * k);
    self->axes[2].x = a2.x;
    self->axes[2].y = a2.y;
    self->axes[2].z = a2.z;
    k = self->halfExtents.z;
    a2.x = a2.x * k;
    a2.y = a2.y * k;
    a2.z = a2.z * k;

    /* corners around centre: c0 = - - -, c4 = - - +, c3 = - + -, c7 = - + +,
       c1 = + - -, c5 = + - +, c2 = + + -, c6 = + + + (signs of a0, a1, a2) */
    lo.x = centre->x - a0.x;
    lo.y = centre->y - a0.y;
    lo.z = centre->z - a0.z;
    t.x = lo.x - a1.x;
    t.y = lo.y - a1.y;
    t.z = lo.z - a1.z;
    nc[0].x = t.x - a2.x;
    nc[0].y = t.y - a2.y;
    nc[0].z = t.z - a2.z;
    nc[4].x = t.x + a2.x;
    nc[4].y = t.y + a2.y;
    nc[4].z = t.z + a2.z;
    t.x = lo.x + a1.x;
    t.y = lo.y + a1.y;
    t.z = lo.z + a1.z;
    nc[3].x = t.x - a2.x;
    nc[3].y = t.y - a2.y;
    nc[3].z = t.z - a2.z;
    nc[7].x = t.x + a2.x;
    nc[7].y = t.y + a2.y;
    nc[7].z = t.z + a2.z;
    hi.x = centre->x + a0.x;
    hi.y = centre->y + a0.y;
    hi.z = centre->z + a0.z;
    t.x = hi.x - a1.x;
    t.y = hi.y - a1.y;
    t.z = hi.z - a1.z;
    nc[1].x = t.x - a2.x;
    nc[1].y = t.y - a2.y;
    nc[1].z = t.z - a2.z;
    nc[5].x = t.x + a2.x;
    nc[5].y = t.y + a2.y;
    nc[5].z = t.z + a2.z;
    t.x = hi.x + a1.x;
    t.y = hi.y + a1.y;
    t.z = hi.z + a1.z;
    nc[2].x = t.x - a2.x;
    nc[2].y = t.y - a2.y;
    nc[2].z = t.z - a2.z;
    nc[6].x = t.x + a2.x;
    nc[6].y = t.y + a2.y;
    nc[6].z = t.z + a2.z;

    /* self->pos is re-read before every store; only x/y/z are written (`.t` ops) */
    for (i = 0; i < 8; i++) {
        self->pos[i].x = nc[i].x;
        self->pos[i].y = nc[i].y;
        self->pos[i].z = nc[i].z;
    }
}
