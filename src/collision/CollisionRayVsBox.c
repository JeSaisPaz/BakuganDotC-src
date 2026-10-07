// bdc 0x089e8668 CollisionRayVsBox
#include "bdc.h"

/* Ray shape method for box targets (type-1 vtable slot 4): forwards to the box's "vs ray" method
   (`CollisionBoxVsRay`). */

bool CollisionRayVsBox(CollisionRayShape *ray, void *other)

{
  const VtblEntry *entry = ((CollisionRayShape *)other)->vtbl + 1;
  return ((bool (*)(void *, CollisionRayShape *))entry->fn)((char *)other + entry->delta, ray);
}
