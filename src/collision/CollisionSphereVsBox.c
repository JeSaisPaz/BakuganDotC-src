// bdc 0x089e92f4 CollisionSphereVsBox
#include "bdc.h"

/* Sphere shape method for box queries (type-3 slot 4): forwards to the box's "vs sphere" method
   (`CollisionBoxVsSphere`). */

bool CollisionSphereVsBox(void *sphere, void *box)

{
  const VtblEntry *e = &((CollisionBox *)box)->vtbl[3];

  return ((bool (*)(void *, void *))e->fn)((char *)box + e->delta, sphere);
}

