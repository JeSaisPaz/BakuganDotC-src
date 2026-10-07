// bdc 0x088f4824 GameFieldCharSetRestartActor
#include "bdc.h"

/* Restarts the placed behaviour of the actor in slot `slot` (`ActorRestartPlacedBehaviour`). */

void GameFieldCharSetRestartActor(void *mgr, u8 slot)

{
  ActorRestartPlacedBehaviour(((Actor **)mgr)[slot]);
  return;
}
