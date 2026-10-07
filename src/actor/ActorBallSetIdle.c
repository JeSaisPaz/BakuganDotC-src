// bdc 0x088b87f8 ActorBallSetIdle
#include "bdc.h"

/* Puts an `ActorBall` (`ActorBallCtor`) back into state 0: clears the in-flight flag `+0x1f4`,
   `+0x6c` and the travelled distance `+0x1f8`. */

void ActorBallSetIdle(CoreObject *ball)

{
  ActorBall *b = (ActorBall *)ball;

  b->inFlight = 0;
  b->base.ambient[3] = 0.0f;
  b->travelled = 0.0f;
  ActorBallSetState(ball, 0, false);
  return;
}
