// bdc 0x088f2a30 GameEventCamTweenDtor
#include "bdc.h"

/* Destructor of the camera tween action (vtable `0x08af430c`, 0x10 bytes) (slot 1): restores the
   action base vtable `0x08af6e20` and frees the object when `flags & 1`. */

void GameEventCamTweenDtor(GameEventCamTween *self, u32 flags)

{
  if ((self != (GameEventCamTween *)0x0) &&
     ((self->base).vtbl = g_gameEventActionVtbl, (flags & 1) != 0)) {
    MemLock();
    MemFree(self,(char *)0x0,0);
    MemUnlock();
  }
  return;
}

