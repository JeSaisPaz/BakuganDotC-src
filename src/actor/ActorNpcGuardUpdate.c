// bdc 0x088e94a4 ActorNpcGuardUpdate
#include "bdc.h"

/* Per-frame update of the guard classes (slot 34): `ActorNpcUpdate`, then outside states 5 and 10
   restores the normal view (slot 46 with 1) once when the look mode `+0x450` was changed. */

typedef struct GuardVtbl {
  u8 head[0x170];
  s16 adj;
  void (*fn)(void *self, s32 arg);
} GuardVtbl;

void ActorNpcGuardUpdate(ActorNpcGuard *self)

{
  int state;
  const GuardVtbl *entry;
  
  ActorNpcUpdate(&self->base);
  state = (self->base).aiState;
  if (((state != 5) && (state != 10)) && (self->lookMode != 0)) {
    entry = (const GuardVtbl *)(self->base).base.base.base.vtable;
    entry->fn((u8 *)self + entry->adj,1);
    self->lookMode = 0;
  }
  return;
}

