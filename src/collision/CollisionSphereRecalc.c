// bdc 0x08a297a0 CollisionSphereRecalc
#include "bdc.h"

/* Recomputes the derived data of a collision sphere: radius squared (`+0x1c`) from the radius
   (`+0x20`). */

void CollisionSphereRecalc(void *sphere)

{
  CollisionSphereQuery *s = (CollisionSphereQuery *)sphere;

  s->center.w = /* radius squared lives in the centre's w slot */ s->radius * s->radius;
  return;
}

