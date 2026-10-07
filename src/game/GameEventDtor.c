// bdc 0x088eb608 GameEventDtor
#include "bdc.h"

/* Destructor (vtable slot 1) of the field event task base class: reinstalls the base vtable
   `0x08af41ec`, runs `CoreTaskDestroy``(task, 0)` and frees the object when `flags & 1`. */

void GameEventDtor(GameEvent *self, u32 flags)

{
  if (self != (GameEvent *)0x0) {
    (self->base).vtable = g_gameEventVtbl;
    CoreTaskDestroy(&self->base,0);
    if ((flags & 1) != 0) {
      MemLock();
      MemFree(self,(char *)0x0,0);
      MemUnlock();
    }
  }
  return;
}

