// bdc 0x08a323e8 CollisionSphereSetRadius
#include "bdc.h"

/* Sets the radius of a sphere collision shape (vtable `0x08af55c4` entry 10, offset `+0x54`):
   stores `radius` at `+0x20` and calls the shape's recalc virtual (`+0x4c`,
   `CollisionSphereRecalc` for spheres: radius² at `+0x1c`). */

void CollisionSphereSetRadius(void *shape, float radius)

{
  CollisionSphereQuery *s = (CollisionSphereQuery *)shape;
  const VtblEntry *e;

  s->radius = radius;
  e = &s->vtbl[9];
  ((void (*)(void *))e->fn)((char *)shape + e->delta);
  return;
}

