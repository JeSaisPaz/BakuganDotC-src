// bdc 0x088e6038 ActorDistSqTo
#include "bdc.h"

/* Squared 3D distance from the actor position `+0x20` to `pos`. */

float ActorDistSqTo(Actor *actor, const float *pos)
{
  float dx = pos[0] - actor->base.pos[0];
  float dy = pos[1] - actor->base.pos[1];
  float dz = pos[2] - actor->base.pos[2];

  return dx * dx + dy * dy + dz * dz;
}
