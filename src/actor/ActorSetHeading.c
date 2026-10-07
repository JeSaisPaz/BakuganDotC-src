// bdc 0x088dea9c ActorSetHeading
#include "bdc.h"

/* Stores a heading angle (radians; the 4th component of the spawn position record) in the actor's
   `base.rot[1]` (+0x34) and in the heading of its input helper (`BtlInput`). */

void ActorSetHeading(Actor *self, float heading)

{
  BtlInput *input;

  input = (BtlInput *)self->input;
  self->base.rot[1] = heading;
  input->heading = heading;
}
