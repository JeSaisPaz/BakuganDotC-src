// bdc 0x088f484c GameFieldCharSetFreezeActor
#include "bdc.h"

/* Calls virtual slot 16 (freeze; `ActorNpcFreeze` for NPCs) on the actor in slot `slot`. */

void GameFieldCharSetFreezeActor(void *mgr, u8 slot)

{
  Actor *actor;
  const VtblEntry *entry;

  actor = ((Actor **)mgr)[slot];
  entry = (const VtblEntry *)actor->base.base.vtable + 16;
  ((void (*)(void *))entry->fn)((char *)actor + entry->delta);
  return;
}
