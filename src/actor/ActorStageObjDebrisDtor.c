// bdc 0x088b5344 ActorStageObjDebrisDtor
#include "bdc.h"

/* Destructor (vtable `g_actorStageObjDebrisVtable` slot 1) of the break-debris model
   (`ActorStageObjDebrisCtor`): does nothing for NULL; otherwise reinstalls its vtable, destroys
   the `base.partCount` fragments (`CollisionPhysBox`, virtual destructor slot 1 with flags 2),
   frees the fragment block (0x10-byte array header before `fragments`) and `fragLast` (cleared),
   and runs `GfxModelDtor`. GCC 2.x deleting destructor: frees the object when bit 0 of `flags`
   is set. */

void ActorStageObjDebrisDtor(ActorStageObjDebris *self, u32 flags)
{
  CollisionPhysBox *frag;
  const VtblEntry *dtor;
  CxxVecBlock *block;
  s32 i;

  if (self == NULL) {
    return;
  }
  self->base.base.vtable = &g_actorStageObjDebrisVtable;
  for (i = 0; i < self->base.partCount; i++) {
    frag = &self->fragments[i];
    dtor = &frag->vtbl[1];
    ((void (*)(void *, u32))dtor->fn)((u8 *)frag + dtor->delta, 2);
  }
  block = (CxxVecBlock *)self->fragments - 1; /* new[] cookie header in front of the array */ /* bdc: record-view ok: the header before the array, not a view of it */
  if (block != NULL) {
    MemLock();
    MemFree(block, NULL, 0);
    MemUnlock();
  }
  if (self->fragLast != NULL) {
    MemLock();
    MemFree(self->fragLast, NULL, 0);
    MemUnlock();
    self->fragLast = NULL;
  }
  GfxModelDtor(&self->base, 0);
  if ((flags & 1) != 0) {
    MemLock();
    MemFree(self, NULL, 0);
    MemUnlock();
  }
}
