// bdc 0x089e69e4 CollisionPhysBoxNudgeRandomCorner
#include "bdc.h"

/* Adds `delta` (xyz) to the velocity of one random corner of the physics box: the corner index is the
   top 3 bits of a random 32-bit integer (`vrndi`). The corner's `w` is left as it was. */
void CollisionPhysBoxNudgeRandomCorner(CollisionPhysBox *self, const float *delta)
{
    ScePspFVector4 *vel = self->vel;
    u32 rnd = PlatformRandU32();
    ScePspFVector4 *corner = vel + (rnd >> 29);

    corner->x = corner->x + delta[0];
    corner->y = corner->y + delta[1];
    corner->z = corner->z + delta[2];
}
