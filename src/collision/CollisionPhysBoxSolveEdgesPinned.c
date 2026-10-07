// bdc 0x089e6c08 CollisionPhysBoxSolveEdgesPinned
#include "bdc.h"

/* Variant of `CollisionPhysBoxSolveEdges` that respects pinned corners: the same correction
   d * (1 / |d|) * ((restLen - |d|) * 0.5) (d = a - b) is computed for each of the 28 pairs, but a gains
   it only if `pins[ia]` is 0 and b loses it only if `pins[ib]` is 0 (x, y, z only; w kept). */

void CollisionPhysBoxSolveEdgesPinned(CollisionPhysBox *self, s32 iterations)
{
    s32 it;
    s32 i;

    for (it = 0; it < iterations; it++) {
        for (i = 0; i < 28; i++) {
            u8 ia = g_collisionPhysBoxEdges[i * 2];
            u8 ib = g_collisionPhysBoxEdges[i * 2 + 1];
            ScePspFVector4 a = self->pos[ia];
            ScePspFVector4 b = self->pos[ib];
            float dx = a.x - b.x;
            float dy = a.y - b.y;
            float dz = a.z - b.z;
            float len = __builtin_sqrtf(dx * dx + dy * dy + dz * dz);
            float half = (self->restLen[i] - len) * 0.5f;
            float k = (1.0f / len) * half;

            dx = dx * k;
            dy = dy * k;
            dz = dz * k;
            if (self->pins[ia] == 0) {
                a.x = a.x + dx;
                a.y = a.y + dy;
                a.z = a.z + dz;
                self->pos[ia] = a;
            }
            if (self->pins[ib] == 0) {
                b.x = b.x - dx;
                b.y = b.y - dy;
                b.z = b.z - dz;
                self->pos[ib] = b;
            }
        }
    }
}
