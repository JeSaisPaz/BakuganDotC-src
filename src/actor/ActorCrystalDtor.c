// bdc 0x0885655c ActorCrystalDtor
#include "bdc.h"

/* Destructor of the crystal actor (`g_actorCrystalVtable` slot 1): deletes the second collider
   `collider2` and the aux object `auxObject` (virtual destructor, flags 3), frees `collisionBox`,
   then chains to `BtlBakuganDtor` and frees itself when `flags & 1` (GCC 2.x deleting
   destructor). */

void ActorCrystalDtor(ActorCrystal *self, u32 flags)
{
    CollisionCollider *collider;
    CoreNode *aux;
    void *box;
    const VtblEntry *dtor;

    if (self != NULL) {
        collider = (CollisionCollider *)self->collider2;
        self->base.base.base.vtable = g_actorCrystalVtable;
        if (collider != NULL) {
            dtor = &((const VtblEntry *)collider->node.vtable)[1];
            ((void (*)(void *, s32))dtor->fn)((u8 *)collider + dtor->delta, 3);
            self->collider2 = NULL;
        }
        aux = (CoreNode *)self->auxObject;
        if (aux != NULL) {
            dtor = &((const VtblEntry *)aux->vtable)[1];
            ((void (*)(void *, s32))dtor->fn)((u8 *)aux + dtor->delta, 3);
            self->auxObject = NULL;
        }
        box = self->collisionBox;
        if (box != NULL) {
            MemLock();
            MemFree(box, NULL, 0);
            MemUnlock();
            self->collisionBox = NULL;
        }
        BtlBakuganDtor(&self->base, 0);
        if ((flags & 1) != 0) {
            MemLock();
            MemFree(self, NULL, 0);
            MemUnlock();
        }
    }
}
