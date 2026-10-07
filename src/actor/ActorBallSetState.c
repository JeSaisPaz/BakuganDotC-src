// bdc 0x088b8784 ActorBallSetState
#include "bdc.h"

/* Sets the state `+0x150` of an `ActorBall` (`ActorBallCtor`); unless `keepStep` is set also
   clears the step `+0x154` and the timer `+0x158`. */

void ActorBallSetState(CoreObject *ball, s32 state, bool keepStep)

{
  ActorBall *b = (ActorBall *)ball;

  b->state = state;
  if (!keepStep) {
    b->step = 0;
    b->timer = 0;
  }
  return;
}
