// bdc 0x0889cac0 BtlStageCreateMapCollider
#include "bdc.h"

/* Creates the arena collider: allocates a 400-byte `CollisionCollider` from the low end of the
   heap (`MemSetAllocFromLow`, previous policy restored), constructs it with
   `CollisionColliderCtor` flags 2, loads mesh `name` from the package chain `g_ioLzsPackages`
   (`CorePackChainFind`, `CollisionColliderInitMesh` layer 8), clears its `byte104` (`+0x104`)
   and sets bit 0 of `flags` (`+0x130`). Returns the collider. Used by `BtlStageLoadMap` for
   `battle_map.ctc`. */

CoreNode *BtlStageCreateMapCollider(const char *name)
{
    bool fromLow;
    CollisionCollider *mem;
    CollisionCollider *collider;
    void *mesh;

    MemLock();
    fromLow = MemIsAllocFromLow();
    MemSetAllocFromLow(true);
    mem = (CollisionCollider *)MemAlloc(sizeof(CollisionCollider), NULL, 0);
    MemSetAllocFromLow(fromLow);
    MemUnlock();
    collider = NULL;
    if (mem != NULL) {
        CollisionColliderCtor((CoreNode *)mem, 2);
        collider = mem;
    }
    mesh = CorePackChainFind(g_ioLzsPackages, (char *)name);
    CollisionColliderInitMesh((CoreNode *)collider, mesh, 8, 0, 0);
    collider->byte104 = 0;
    collider->flags = collider->flags | 1;
    return (CoreNode *)collider;
}
