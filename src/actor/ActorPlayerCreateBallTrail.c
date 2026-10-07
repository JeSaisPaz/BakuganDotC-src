// bdc 0x088e3cb8 ActorPlayerCreateBallTrail
#include "bdc.h"

/* Allocates (low heap) the 0x840-byte trail object of the thrown object (`ActorBallTrailCtor`)
   into `+0x4a4`, or resets the existing one (`ActorBallTrailReset(trail, 0)`). Called by
   `ActorPlayerCtor`. */

void ActorPlayerCreateBallTrail(ActorPlayer *self)

{
  bool fromLow;
  void *trail;
  void *result;
  
  if (self->trail == (void *)0x0) {
    MemLock();
    fromLow = MemIsAllocFromLow();
    MemSetAllocFromLow(true);
    trail = MemAlloc(sizeof(ActorBallTrail),(char *)0x0,0);
    MemSetAllocFromLow(fromLow);
    MemUnlock();
    result = (void *)0x0;
    if (trail != (void *)0x0) {
      ActorBallTrailCtor(trail);
      result = trail;
    }
    self->trail = result;
  }
  else {
    ActorBallTrailReset(self->trail,false);
  }
  return;
}
