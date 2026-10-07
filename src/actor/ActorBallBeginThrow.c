// bdc 0x088b87a0 ActorBallBeginThrow
#include "bdc.h"

/* Puts an `ActorBall` (`ActorBallCtor`) into state 1 (`ActorBallStateThrown`): resets scale to
   0.2, clears the freeze/flag bytes `+0x1d1..+0x1d3`, the hit type `+0x1f0` and the travelled
   distance `+0x1f8`. */

void ActorBallBeginThrow(CoreObject *ball)

{
  ActorBall *b = (ActorBall *)ball;

  ActorBallSetState(ball, 1, false);
  b->base.scale[3] = 0.0f;
  b->base.scale[0] = 0.2f;
  b->base.scale[1] = 0.2f;
  b->base.scale[2] = 0.2f;
  b->flag1d1 = 0;
  b->hitType = 0;
  b->flag1d2 = 0;
  b->flag1d3 = 0;
  b->travelled = 0.0f;
}
