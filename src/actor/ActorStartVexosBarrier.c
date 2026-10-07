// bdc 0x08859a44 ActorStartVexosBarrier
#include "bdc.h"

/* Turns a battle crystal's Vexos barrier on (only while `inBattle` is set; otherwise does
   nothing): spawns effect 0x3b (`barrierVariant != 0`) or 0x157 on `g_btlUnitEffectMgr` at the
   crystal's position, allocates a 400-byte collider from the low heap
   (`CollisionColliderCtor(.., 3)`), stores it in `auxObject` and gives it the mesh of
   `"fz_VexosBarrier.ctc"` (or `"fz_VexosBarrier60.ctc"` when `barrierVariant` is 0) on layer 9,
   owned by the crystal and attached to `facingMatrix`. Then rebuilds the crystal's matrices,
   enables `collider0` (flags `|= 1|0x40|4`, `hitTimer = 0`) and calls vtable slot 23. */

void ActorStartVexosBarrier(Actor *self)

{
  ActorCrystal *crystal = (ActorCrystal *)self;
  bool fromLow;
  CollisionCollider *mem;
  CollisionCollider *collider;
  void *mesh;
  char *name;
  const VtblEntry *slot;

  if (crystal->inBattle != 0) {
    if (crystal->barrierVariant == 0) {
      GfxEffectSpawnWithOwner(g_btlUnitEffectMgr,0x157,crystal->base.base.pos,self);
    }
    else {
      GfxEffectSpawnWithOwner(g_btlUnitEffectMgr,0x3b,crystal->base.base.pos,self);
    }
    collider = (CollisionCollider *)0x0;
    MemLock();
    fromLow = MemIsAllocFromLow();
    MemSetAllocFromLow(true);
    mem = MemAlloc(400,(char *)0x0,0);
    MemSetAllocFromLow(fromLow);
    MemUnlock();
    if (mem != (CollisionCollider *)0x0) {
      CollisionColliderCtor(&mem->node,3);
      collider = mem;
    }
    crystal->auxObject = collider;
    if (crystal->barrierVariant == 0) {
      name = "fz_VexosBarrier60.ctc";
    }
    else {
      name = "fz_VexosBarrier.ctc";
    }
    collider = (CollisionCollider *)crystal->auxObject;
    mesh = CorePackChainFind(g_ioLzsPackages,name);
    CollisionColliderInitMesh(&collider->node,mesh,9,self,0);
    collider = (CollisionCollider *)crystal->auxObject;
    collider->flags = collider->flags | 1;
    ((CollisionCollider *)crystal->auxObject)->byte104 = 0;
    ((CollisionCollider *)crystal->auxObject)->hitField144 = 1;
    ((CollisionCollider *)crystal->auxObject)->owner = self;
    collider = (CollisionCollider *)crystal->auxObject;
    collider->hitTimer = -1;
    collider->flags = collider->flags | 1;
    collider = (CollisionCollider *)crystal->auxObject;
    collider->attachMatrix = crystal->facingMatrix;
    collider->attachDirty = 1;
    ActorCrystalUpdateMatrices(crystal);
    crystal->base.collider0->hitTimer = 0;
    crystal->base.collider0->flags = crystal->base.collider0->flags | 1;
    crystal->base.collider0->flags = crystal->base.collider0->flags | 0x40;
    crystal->base.collider0->flags = crystal->base.collider0->flags | 4;
    slot = (const VtblEntry *)crystal->base.base.base.vtable + 23;
    ((void (*)(void *))slot->fn)((u8 *)self + slot->delta);
  }
  return;
}
