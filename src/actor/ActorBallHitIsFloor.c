// bdc 0x088b8c60 ActorBallHitIsFloor
#include "bdc.h"

/* Returns whether the last hit normal of an `ActorBall` (`+0x170`, y at `+0x174`, set by
   `ActorBallCheckHit`) points up enough (!(y <= 0.3)) to count as floor. */

bool ActorBallHitIsFloor(CoreObject *ball)

{
  return !(((ActorBall *)ball)->hitNormal[1] <= 0.3f);
}
