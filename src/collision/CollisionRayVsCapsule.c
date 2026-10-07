// bdc 0x089e8634 CollisionRayVsCapsule
#include "bdc.h"

/* Ray shape method for capsule targets (type-1 vtable slot 3): forwards to the capsule's "vs ray"
   method (`CollisionCapsuleVsRay`). */

bool CollisionRayVsCapsule(CollisionRayShape *ray, void *other)

{
  const VtblEntry *entry = ((CollisionRayShape *)other)->vtbl + 1;
  return ((bool (*)(void *, CollisionRayShape *))entry->fn)((char *)other + entry->delta, ray);
}
