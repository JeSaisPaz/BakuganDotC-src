// bdc 0x088b877c ActorBallSetHeading
#include "bdc.h"

/* Stores `heading` (radians) into the yaw angle `rot[1]` of the model; used for ActorBalls and for
   battle units in the demo/entry code. */
void ActorBallSetHeading(float heading, GfxModel *model)
{
    model->rot[1] = heading;
}
