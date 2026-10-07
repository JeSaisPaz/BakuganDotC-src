// bdc 0x088eabe0 GameEventActionListDtor
#include "bdc.h"

/* Destructor of the event action list task (vtable `g_gameEventActionListVtbl` slot 1): `CoreTaskDestroy` and
   free when `flags & 1`. */

void GameEventActionListDtor(GameEventActionList *self, u32 flags)

{
  if (self != (GameEventActionList *)0x0) {
    (self->base).vtable = g_gameEventActionListVtbl;
    CoreTaskDestroy(&self->base,0);
    if ((flags & 1) != 0) {
      MemLock();
      MemFree(self,(char *)0x0,0);
      MemUnlock();
    }
  }
  return;
}

