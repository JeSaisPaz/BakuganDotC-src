// bdc 0x08a2c510 ActorNpcRunState
#include "bdc.h"

/* Runs the field NPC's current state (vtable entry 30, offset `+0xf4`, shared by the NPC
   `0x08af3b74`, cloaked guard `0x08af39e4`, switch robot `0x08af3d04`, robot `0x08af3e94` and guard
   `0x08af4024` vtables): calls the `MemberFnPtr` `0x08a98d30[npc+0x3a0]` on `npc + delta`
   (virtual when `vindex != 0`). */

void ActorNpcRunState(ActorNpc *self)

{
  const MemberFnPtr *member = &g_actorNpcStateFns[self->aiState];
  u8 *obj = (u8 *)self + member->delta;
  void *fn = member->pfn;

  if (member->index != 0) {
    const VtblEntry *entry =
        &(*(const VtblEntry **)(obj + (uintptr_t)member->pfn))[member->index];

    fn = entry->fn;
    obj += entry->delta;
  }
  ((void (*)(void *))fn)(obj);
  return;
}
