// bdc 0x088a3a9c ActorCrystalStandSyncCollider
#include "bdc.h"

/* Binds the stand's collider `+0x140` to the model matrix (`collider+0x110 = model+0x130 + 0x80`)
   and marks it dirty (`+0x10c = 1`). Called by `ActorCrystalUpdateFade`. */

void ActorCrystalStandSyncCollider(ActorCrystalStand *stand)
{
  CollisionCollider *collider = stand->collider;

  collider->attachMatrix = (Mat4Row *)stand->base.data->rootMatrix;
  collider->attachDirty = 1;
}
