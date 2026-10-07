// bdc 0x088a37b4 ActorCrystalStandSetCollidable
#include "bdc.h"

/* Switches the crystal stand's collider `+0x140`: `passThrough` true sets collider kind 9 and
   clears flags 1/4/0x40 (no collision), false sets kind 7 and sets flags 1/4/0x40 (solid). Called
   by `ActorCrystalUpdate`, `ActorCrystalUpdateMatrices` and `ActorCrystalStandCheckOrigin`.
 */

void ActorCrystalStandSetCollidable(ActorCrystalStand *stand, char passThrough)
{
    if (stand->collider == NULL)
        return;
    if (passThrough != 0) {
        stand->collider->layer = 9;
        stand->collider->hitTimer = 0;
        stand->collider->flags &= ~1u;
        stand->collider->flags &= ~0x40u;
        stand->collider->flags &= ~4u;
        return;
    }
    stand->collider->layer = 7;
    stand->collider->hitTimer = 0;
    stand->collider->flags |= 1;
    stand->collider->flags |= 0x40;
    stand->collider->flags |= 4;
}
