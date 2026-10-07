// bdc 0x088a3710 ActorCrystalStandDtor
#include "bdc.h"

/* Destructor (vtable `0x08af24d4` slot 1) of the crystal stand model (`ActorCrystalStandCtor`):
   destroys the collider `+0x140` (virtual destructor, flags 3) and runs `GfxModelDtor`. (GCC 2.x
   deleting destructor: frees the object when bit 0 of `flags` is set). */

void ActorCrystalStandDtor(ActorCrystalStand *obj, u32 flags)
{
    CollisionCollider *collider;
    const VtblEntry *dtor;

    if (obj != NULL) {
        collider = obj->collider;
        obj->base.base.vtable = g_actorCrystalStandVtable;
        if (collider != NULL) {
            dtor = &((const VtblEntry *)collider->node.vtable)[1];
            ((void (*)(void *, s32))dtor->fn)((u8 *)collider + dtor->delta, 3);
            obj->collider = NULL;
        }
        GfxModelDtor(&obj->base, 0);
        if ((flags & 1) != 0) {
            MemLock();
            MemFree(obj, NULL, 0);
            MemUnlock();
        }
    }
}
