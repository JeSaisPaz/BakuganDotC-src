// bdc 0x089d87fc CoreObjectAppend
#include "bdc.h"

/* Appends `obj` to the end of the sibling chain that `chain` belongs to: walks `chain->next` to the
   last element, makes that element point to `obj`, sets `obj->prev` to it and `obj->next = NULL`.
   With `chain == NULL` the object simply starts a new chain (`prev = next = NULL`). Called by
   `CoreObjectInit` and by three more constructors (`CollisionDebugPrimLink`, `GfxSpriteLayerAllocPoolSlot`,
   `GfxTextureInitFromTim2`). */
void CoreObjectAppend(CoreObject *obj, CoreObject *chain)
{
    if (chain == NULL) {
        obj->prev = NULL;
        obj->next = NULL;
        return;
    }
    while (chain->next != NULL) {
        chain = chain->next;
    }
    chain->next = obj;
    obj->prev = chain;
    obj->next = NULL;
}
