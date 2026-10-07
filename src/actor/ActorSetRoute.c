// bdc 0x088df754 ActorSetRoute
#include "bdc.h"

/* Sets the placement route mode (`record+0x39`): mode 0 clears `record+0x12`; mode 1 selects route
   `route` (`record+0x3b`, reloading it with `ActorLookupRoute` unless it is already loaded), sets
   `+0x391 = !noLoop` and switches to behaviour 9 (follow route). */

void ActorSetRoute(Actor *self, u8 mode, u8 route, u8 noLoop)

{
  ActorNpcPlacement *place;

  ((ActorNpcPlacement *)self->placement)->routeMode = mode;
  place = (ActorNpcPlacement *)self->placement;
  if (place->routeMode == 0) {
    place->routeFlag = 0;
  } else if (place->routeMode < 2) {
    if (self->route != (s32 *)0x0) {
      if (route == place->routeIndex) {
        return;
      }
      self->route = (s32 *)0x0;
    }
    place->routeIndex = route;
    ActorLookupRoute(self);
    self->routeEnds = noLoop == 0;
    ActorSetStateBase(self, 9, 0);
  }
}
