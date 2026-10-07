// bdc 0x088b7a48 ActorBallTrailDtor
#include "bdc.h"

/* Destructor of the ball trail (`ActorBallTrailCtor`): frees the sprite table `+0x830` and, when
   bit 0 of `flags` is set, the trail itself. Called by `ActorPlayerDtor`. */

void ActorBallTrailDtor(void *trail, u32 flags)

{
  ActorBallTrail *t = (ActorBallTrail *)trail;
  void **ptr;

  if (t != NULL) {
    ptr = t->sprites;
    if (ptr != NULL) {
      MemLock();
      MemFree(ptr, NULL, 0);
      MemUnlock();
      t->sprites = NULL;
    }
    if ((flags & 1) != 0) {
      MemLock();
      MemFree(t, NULL, 0);
      MemUnlock();
    }
  }
}
