// bdc 0x088e94a4 ActorNpcGuardUpdate
#include "bdc.h"

/* Per-frame update of the guard classes (slot 34): `ActorNpcUpdate`, then outside states 5 and 10
   restores the normal view (slot 46 with 1) once when the look mode `+0x450` was changed. */

void ActorNpcGuardUpdate(ActorNpcGuard *self)

{
  int state;
  const VtblEntry *entry;
  
  ActorNpcUpdate(&self->base);
  state = (self->base).aiState;
  if (((state != 5) && (state != 10)) && (self->lookMode != 0)) {
    entry = &((const VtblEntry *)(self->base).base.base.base.vtable)[46];
    ((void (*)(void *, s32))entry->fn)((u8 *)self + entry->delta,1);
    self->lookMode = 0;
  }
  return;
}

