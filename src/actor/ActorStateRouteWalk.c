// bdc 0x088e0ab8 ActorStateRouteWalk
#include "bdc.h"

/* Route-walk state handler (vtable slot 29, 7 vtables): unless the field is paused, walks toward
   the route target `+0x370` at speed `+0x154` (virtual slot 18, `ActorWalkToward`, radius 2) and
   on arrival switches to state 9 and advances the step `+0x360`; cancels the player's powers
   (`ActorPlayerCancelPowers`) when this actor touched the player (`+0x356`). While paused it only
   plays idle. */

void ActorStateRouteWalk(Actor *self)

{
  if (!g_gameFieldCharSet->paused) {
    const VtblEntry *walk = &((const VtblEntry *)self->base.base.vtable)[18];
    u8 pushed;
    void *player;

    if (((int (*)(float, float, void *, float *, int, int))walk->fn)(
            self->walkSpeed, 2.0f, (u8 *)self + walk->delta, self->routeTarget, 0, 0) != 0) {
      ActorSetStateBase(self, 9, 0);
      self->waitTimer = 0;
      self->routeStep = self->routeStep + 1;
    }
    pushed = self->pushed;
    if (pushed != 0 && (player = ActorFindPlayer()) != NULL) {
      ActorPlayerCancelPowers(player);
    }
    return;
  }
  ActorPlayMotion(0.2f, self, 0, 1, 0);
  return;
}
