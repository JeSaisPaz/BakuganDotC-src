// bdc 0x089e976c CollisionCapsuleVsBox
#include "bdc.h"

/* Capsule shape method for box queries (type-4 slot 4): forwards to the box's "vs capsule" method
   (vtable entry 4, `CollisionBoxVsCapsule`), adjusting `this` by the entry delta. */

bool CollisionCapsuleVsBox(CollisionCapsule *capsule, CollisionBox *box)

{
  const VtblEntry *entry = &box->vtbl[4];

  return ((bool (*)(void *, CollisionCapsule *))entry->fn)((char *)box + entry->delta, capsule);
}
