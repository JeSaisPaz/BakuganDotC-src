// bdc 0x088f4b5c GameFieldCharSetSetActorRoute
#include "bdc.h"

/* Puts the actor in slot `slot` on route `route` (`ActorSetRoute` mode 1). */

void GameFieldCharSetSetActorRoute(void *mgr, u8 slot, u8 route, u8 noLoop)

{
  ActorSetRoute(((Actor **)mgr)[slot],1,route,noLoop);
  return;
}
