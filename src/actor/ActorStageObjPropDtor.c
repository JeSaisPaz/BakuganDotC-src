// bdc 0x088b0b10 ActorStageObjPropDtor
#include "bdc.h"

/* Destructor (vtable `0x08af2ae4` slot 1) of the knock-over prop (`ActorStageObjPropCtor`):
   chains to `ActorStageObjBaseDtor`. (GCC 2.x deleting destructor: frees the object when bit 0 of
   `flags` is set). */

/* Object at `physicsBox` has its vtable pointer at +0x164. */
typedef struct PhysBoxVtView {
  unsigned char _pad[0x164];
  const void *vtable;
} PhysBoxVtView;

void ActorStageObjPropDtor(ActorStageObjProp *self, u32 flags)

{
  void *box;

  if (self != (ActorStageObjProp *)0x0) {
    box = self->physicsBox;
    (self->base).base.base.vtable = &g_actorStageObjPropVtbl;
    if (box != (void *)0x0) {
      const VtblEntry *ent = (const VtblEntry *)((PhysBoxVtView *)box)->vtable + 1;

      ((void (*)(void *, int))ent->fn)((char *)box + ent->delta, 3);
      self->physicsBox = (void *)0x0;
    }
    ActorStageObjBaseDtor(&self->base,0);
    if ((flags & 1) != 0) {
      MemLock();
      MemFree(self,(char *)0x0,0);
      MemUnlock();
    }
  }
  return;
}

