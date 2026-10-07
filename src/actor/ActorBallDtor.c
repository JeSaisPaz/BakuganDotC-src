// bdc 0x088b8200 ActorBallDtor
#include "bdc.h"

/* Destructor (vtable slot 1) of the `ActorBall` (`ActorBallCtor`): frees the motion slot map
   `+0x144`, runs `GfxModelDtor` and frees the object when `flags & 1`. */

void ActorBallDtor(CoreObject *ball, u32 flags)

{
  ActorBall *b = (ActorBall *)ball;
  void *ptr;

  if (b != NULL) {
    ptr = b->motionMap;
    b->base.base.vtable = &g_actorBallVtable;
    if (ptr != NULL) {
      MemLock();
      MemFree(ptr, NULL, 0);
      MemUnlock();
      b->motionMap = NULL;
    }
    GfxModelDtor(&b->base, 0);
    if ((flags & 1) != 0) {
      MemLock();
      MemFree(b, NULL, 0);
      MemUnlock();
    }
  }
}
