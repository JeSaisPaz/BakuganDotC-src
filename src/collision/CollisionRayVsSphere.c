// bdc 0x089e8600 CollisionRayVsSphere
#include "bdc.h"

/* Ray shape method for sphere targets (type-1 vtable slot 2) (double dispatch through the shape
   vtables `0x08af5504` ray, `0x08af5564` segment, `0x08af55c4` sphere, `0x08af5624` capsule,
   `0x08af5684` box; slot k tests against a query of type k+1, type 6 for slot 4): forwards to the
   sphere's "vs ray" method (slot 0, `CollisionSphereVsRay`) with the arguments swapped. */

bool CollisionRayVsSphere(CollisionRayShape *ray, void *other)

{
  const VtblEntry *entry = ((CollisionRayShape *)other)->vtbl + 1;
  return ((bool (*)(void *, CollisionRayShape *))entry->fn)((char *)other + entry->delta, ray);
}
