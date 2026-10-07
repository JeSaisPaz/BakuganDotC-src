// bdc 0x088efd48 GameEvent470Release
#include "bdc.h"

/* Frees the event-actor records `+0x284` and the event resources (`GameEventRelease`). */

void GameEvent470Release(GameEvent470 *self)

{
  GameEventActorRecord *ptr;
  
  ptr = self->actors;
  if (ptr != (GameEventActorRecord *)0x0) {
    MemLock();
    MemFree(ptr,(char *)0x0,0);
    MemUnlock();
    self->actors = (GameEventActorRecord *)0x0;
  }
  GameEventRelease(&self->base);
  return;
}

