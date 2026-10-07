// bdc 0x088e7448 ActorNpcStateWalkRoute
#include "bdc.h"

/* AI state 2 of the field NPC/guard classes (base `ActorNpcCtor`) (slot 37): walks toward the
   route point `routeTarget` at speed `speed` (virtual slot 18, radius 2); on arrival returns to
   state 0 and advances the route step `routeStep`. */

void ActorNpcStateWalkRoute(ActorNpc *self)
{
  const VtblEntry *entry;
  s32 (*walk)(void *, float *, s32, s32, float, float);

  if (ActorNpcCheckInterrupt(self, 0) == 0) {
    entry = (const VtblEntry *)self->base.base.base.vtable + 18;
    walk = (s32 (*)(void *, float *, s32, s32, float, float))entry->fn;
    if (walk((char *)self + entry->delta, self->base.routeTarget, 0, 0, self->speed, 2.0f) != 0) {
      self->aiState = 0;
      self->subStep = 0;
      self->base.routeStep = self->base.routeStep + 1;
    }
  }
}
