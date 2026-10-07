// bdc 0x088de4bc ActorIsOnSurfaceType4
#include "bdc.h"

/* True when the actor's ground surface type (`+0x1c4`, from `ActorProbeGround`) is 4. */

bool ActorIsOnSurfaceType4(Actor *self)

{
  return self->surfaceType == 4;
}

