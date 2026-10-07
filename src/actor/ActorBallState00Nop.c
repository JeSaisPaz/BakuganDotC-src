// bdc 0x088b8c8c ActorBallState00Nop
#include "bdc.h"

/* Empty idle state (state 0) of the `ActorBall` (member-pointer table `0x08a90898`, run by
   `ActorBallUpdate`); state 1 is `ActorBallStateThrown`. */

void ActorBallState00Nop(CoreObject *ball)
{
    (void)ball;
}
