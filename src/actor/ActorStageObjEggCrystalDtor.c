// bdc 0x088a40ac ActorStageObjEggCrystalDtor
#include "bdc.h"

/* Destructor (vtable `0x08af2524` slot 1) of the egg crystal (`ActorStageObjEggCrystalCtor`):
   destroys the companion unit `+800` when it is still in `g_btlBakuganList`, runs
   `ActorStageObjEggCrystalUnlinkOwner` and `ActorStageObjBaseDtor`. (GCC 2.x deleting
   destructor: frees the object when bit 0 of `flags` is set). */

void ActorStageObjEggCrystalDtor(ActorStageObjEggCrystal *self, u32 flags)

{
  void *companion;
  BtlBakugan *oldUnit;
  
  if (self != (ActorStageObjEggCrystal *)0x0) {
    oldUnit = self->unit;
    (self->base).base.base.vtable = &g_actorStageObjEggCrystalVtbl;
    companion = BtlBakuganListFind(oldUnit);
    self->unit = companion;
    if (companion != (void *)0x0) {
      const VtblEntry *ent = (const VtblEntry *)((const CoreObject *)companion)->vtable + 1;

      ((void (*)(void *, int))ent->fn)((char *)companion + ent->delta, 3);
      self->unit = (void *)0x0;
    }
    ActorStageObjEggCrystalUnlinkOwner(self);
    ActorStageObjBaseDtor(&self->base,0);
    if ((flags & 1) != 0) {
      MemLock();
      MemFree(self,(char *)0x0,0);
      MemUnlock();
    }
  }
  return;
}

