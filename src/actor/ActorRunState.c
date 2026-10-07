// bdc 0x08a2c440 ActorRunState
#include "bdc.h"

/* Runs the base actor's current state (`ActorCtor`, vtable `0x08af37e4` entry 30, offset
   `+0xf4`): calls the `MemberFnPtr` `g_actorStateTable[actor+0x140]` on `actor + delta` (virtual when
   `index != 0`). */

void ActorRunState(Actor *self)

{
  const MemberFnPtr *e = &g_actorStateTable[self->state];
  u8 *obj = (u8 *)self + e->delta;
  void *fn = e->pfn;

  if (e->index != 0) {
    const VtblEntry *entry = &(*(const VtblEntry **)(obj + (intptr_t)e->pfn))[e->index];

    fn = entry->fn;
    obj += entry->delta;
  }
  ((void (*)(void *))fn)(obj);
  return;
}
