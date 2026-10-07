// bdc 0x088625a8 BtlBakuganSetColliderLayer
#include "bdc.h"

/* Writes `layer` to the collision `layer` (tested by `CollisionRaycast`/`CollisionHitQuery`)
   of the unit's two colliders, the body collider `collider0` first, then the push collider
   `collider1` (each only when non-NULL). Called with 5 for non-player units right after
   construction. Returns nothing. */

void BtlBakuganSetColliderLayer(BtlBakugan *bakugan, u32 layer)
{
    if (bakugan->collider0 != NULL) {
        bakugan->collider0->layer = layer;
    }
    if (bakugan->collider1 != NULL) {
        bakugan->collider1->layer = layer;
    }
}
