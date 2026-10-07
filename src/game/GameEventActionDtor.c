// bdc 0x08a2c6d0 GameEventActionDtor
#include "bdc.h"

/* Destructor of the event action base class (vtable `0x08af6e20` at `+0`, entry 1): reinstalls the
   base vtable and frees the object when `flags & 1`. */

void GameEventActionDtor(GameEventAction *self, u32 flags)

{
  if ((self != (GameEventAction *)0x0) &&
     (self->vtbl = g_gameEventActionVtbl, (flags & 1) != 0)) {
    MemLock();
    MemFree(self,(char *)0x0,0);
    MemUnlock();
  }
  return;
}

