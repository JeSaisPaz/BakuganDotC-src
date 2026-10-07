// bdc 0x088f3390 GameEventActorTweenDtor
#include "bdc.h"

/* Destructor of the event-actor tween action (vtable `0x08af4344`, 0xc bytes) (slot 1): restores
   the action base vtable `0x08af6e20` and frees the object when `flags & 1`. */

void GameEventActorTweenDtor(GameEventActorTween *self, u32 flags)

{
  if ((self != (GameEventActorTween *)0x0) &&
     ((self->base).vtbl = g_gameEventActionVtbl, (flags & 1) != 0)) {
    MemLock();
    MemFree(self,(char *)0x0,0);
    MemUnlock();
  }
  return;
}

