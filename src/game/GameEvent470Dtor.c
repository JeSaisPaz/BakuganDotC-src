// bdc 0x088efae0 GameEvent470Dtor
#include "bdc.h"

/* Destructor (vtable slot 1) of task class 470 (field event): reinstalls vtable `0x08af425c`, runs
   the base `GameEventDtor``(task, 0)` and frees the object when `flags & 1`. */

void GameEvent470Dtor(GameEvent470 *self, u32 flags)

{
  if (self != (GameEvent470 *)0x0) {
    (self->base).base.vtable = g_gameEvent470Vtbl;
    GameEventDtor(&self->base,0);
    if ((flags & 1) != 0) {
      MemLock();
      MemFree(self,0,0);
      MemUnlock();
    }
  }
  return;
}

