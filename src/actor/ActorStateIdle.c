// bdc 0x088e09f8 ActorStateIdle
#include "bdc.h"

/* Idle state handler of the actors (vtable slot 20, 6 vtables): unless the field is paused
   (`g_gameFieldCharSet->paused`), counts down the wait timer `+0x324` and then, when the actor has a
   route, switches to state 9 and advances the route step `+0x360`. */

void ActorStateIdle(Actor *self)

{
  if (!g_gameFieldCharSet->paused) {
    if (self->waitTimer < 1) {
      if (self->route != NULL) {
        ActorSetStateBase(self,9,'\0');
        self->routeStep = self->routeStep + 1;
      }
    }
    else {
      self->waitTimer = self->waitTimer + -1;
    }
  }
  return;
}

