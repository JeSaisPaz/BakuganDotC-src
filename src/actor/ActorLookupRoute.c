// bdc 0x088df118 ActorLookupRoute
#include "bdc.h"

/* For a placed actor whose placement record (`+0x350`) has route mode 1 (`routeMode`) and no cached
   route, loads the route file `f<area>_<map>_<route>.nmt` (area/map from
   `g_gameEventFlags` bytes 0 and 2, route index `routeIndex`) from the pack chain into `+0x35c`. */

void ActorLookupRoute(Actor *self)
{
  ActorNpcPlacement *place;
  char name[256];

  place = (ActorNpcPlacement *)self->placement;
  if (place != NULL && self->route == NULL && place->routeMode == 1) {
    sprintf(name, "f%01d_%02d_%03d.nmt", g_gameEventFlags[0], g_gameEventFlags[2], place->routeIndex);
    self->route = CorePackChainFind(g_ioLzsPackages, name);
  }
}
