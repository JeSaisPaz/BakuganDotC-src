// bdc 0x088d3cac GameStageCreateCollider
#include "bdc.h"

/* Creates a collider (400 bytes, `CollisionColliderCtor` kind 2), loads collision data `name`
   from the pack chain into it (`CollisionColliderInitMesh`, mask 8), marks it static and points
   its callback block at `g_gameStageColliderCallbacks`. */

CoreNode *GameStageCreateCollider(char *name)
{
    bool fromLow;
    CoreNode *alloc;
    CollisionCollider *collider = NULL;
    void *mesh;

    MemLock();
    fromLow = MemIsAllocFromLow();
    MemSetAllocFromLow(true);
    alloc = MemAlloc(sizeof(CollisionCollider), NULL, 0);
    MemSetAllocFromLow(fromLow);
    MemUnlock();
    if (alloc != NULL) {
        CollisionColliderCtor(alloc, 2);
        collider = (CollisionCollider *)alloc;
    }
    mesh = CorePackChainFind(g_ioLzsPackages, name);
    CollisionColliderInitMesh((CoreNode *)collider, mesh, 8, NULL, 0);
    collider->byte104 = 0;
    collider->flags |= 1;
    collider->attachMatrix = (float (*)[4])g_gameStageColliderCallbacks;
    collider->attachDirty = 1;
    return (CoreNode *)collider;
}
