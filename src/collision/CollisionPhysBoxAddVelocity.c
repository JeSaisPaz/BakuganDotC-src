// bdc 0x089e6a28 CollisionPhysBoxAddVelocity
#include "bdc.h"

/* Adds `delta` (xyz) to the velocity of all eight corners of the physics box; each velocity's w
   is left unchanged. */

void CollisionPhysBoxAddVelocity(CollisionPhysBox *self, const float *delta)

{
  int i;

  for (i = 0; i < 8; i++) {
    ScePspFVector4 *v = &self->vel[i];
    v->x = VfAdd(v->x, delta[0]);
    v->y = VfAdd(v->y, delta[1]);
    v->z = VfAdd(v->z, delta[2]);
  }
}
