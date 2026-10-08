// bdc 0x088a1fb0 ActorStageObjLandmarkDtor
#include "bdc.h"

/* Destructor (`g_actorStageObjLandmarkVtbl` slot 1) of the landmark stage object
   (`ActorStageObjLandmarkCtor`): stops its owned effects (`ActorStageObjStopOwnedEffects`) and
   the attached effect (`ActorStageObjStopEffect`), destroys the helper collider (virtual delete,
   flags 3), the companion unit (virtual delete, only if still in `g_btlBakuganList`,
   `BtlBakuganListFind`) and frees the collision mesh, then `ActorStageObjBaseDtor`. (GCC 2.x
   deleting destructor: frees the object when bit 0 of `flags` is set). */

void ActorStageObjLandmarkDtor(ActorStageObjLandmark *self, u32 flags)
{
  const VtblEntry *dtor;
  BtlTargetPointLandmark *unit;

  if (self == NULL)
    return;
  self->base.base.base.vtable = g_actorStageObjLandmarkVtbl;
  ActorStageObjStopOwnedEffects(&self->base);
  ActorStageObjStopEffect(self);
  if (self->helper != NULL) {
    CollisionCollider *helper = self->helper;
    dtor = &((const VtblEntry *)helper->node.vtable)[1];
    ((void (*)(void *, u32))dtor->fn)((u8 *)helper + dtor->delta, 3);
    self->helper = NULL;
  }
  unit = BtlBakuganListFind(&self->unit->base);
  self->unit = unit;
  if (unit != NULL) {
    dtor = &((const VtblEntry *)unit->base.base.base.vtable)[1];
    ((void (*)(void *, u32))dtor->fn)((u8 *)unit + dtor->delta, 3);
    self->unit = NULL;
  }
  if (self->collisionMesh != NULL) {
    void *mesh = self->collisionMesh;
    MemLock();
    MemFree(mesh, NULL, 0);
    MemUnlock();
    self->collisionMesh = NULL;
  }
  ActorStageObjBaseDtor(&self->base, 0);
  if ((flags & 1) != 0) {
    MemLock();
    MemFree(self, NULL, 0);
    MemUnlock();
  }
}
