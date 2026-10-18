// bdc 0x089e6b58 CollisionPhysBoxSolveEdges
#include "bdc.h"

/* Runs `iterations` passes of distance-constraint relaxation over the 28 corner pairs of
   `g_collisionPhysBoxEdges`: each pair (a, b) is moved apart along d = a - b by
   d * (1 / |d|) * ((restLen - |d|) * 0.5), a gaining and b losing it (x, y, z only; w kept). */

void CollisionPhysBoxSolveEdges(CollisionPhysBox *self, s32 iterations)
{
    s32 it;
    s32 i;

    for (it = 0; it < iterations; it++) {
        for (i = 0; i < 28; i++) {
            u8 ia = g_collisionPhysBoxEdges[i * 2];
            u8 ib = g_collisionPhysBoxEdges[i * 2 + 1];
            ScePspFVector4 a = self->pos[ia];
            ScePspFVector4 b = self->pos[ib];
            float dx = VfSub(a.x, b.x);
            float dy = VfSub(a.y, b.y);
            float dz = VfSub(a.z, b.z);
            float len = VfSqrt(VfDot3(dx, dy, dz, dx, dy, dz));
            float half = VfMul(VfSub(self->restLen[i], len), 0.5f);
            float k = VfMul((VfRcp(len)), half);

            dx = VfMul(dx, k);
            dy = VfMul(dy, k);
            dz = VfMul(dz, k);
            a.x = VfAdd(a.x, dx);
            a.y = VfAdd(a.y, dy);
            a.z = VfAdd(a.z, dz);
            b.x = VfSub(b.x, dx);
            b.y = VfSub(b.y, dy);
            b.z = VfSub(b.z, dz);
            self->pos[ia] = a;
            self->pos[ib] = b;
        }
    }
}
