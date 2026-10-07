// bdc 0x088e0f08 ActorPlayerDtor
#include "bdc.h"

/* Destructor of the player actor (edit-man, `ActorPlayerCtor`), vtable `0x08af38e4` slot 1:
   destroys the ball `+0x418` (`ActorBallCreate`) and the trail `+0x4a4`, then runs `ActorDtor`.
    */

typedef struct PlayerBallVtbl {
  u8 head[8];
  s16 adjust;
  s16 pad;
  void (*fn)(void *, s32);
} PlayerBallVtbl;

void ActorPlayerDtor(ActorPlayer *self, u32 flags)

{
  CoreObject *ball;
  const PlayerBallVtbl *entry;

  if (self != (ActorPlayer *)0x0) {
    ball = (CoreObject *)self->ball;
    (self->base).base.base.vtable = &g_actorPlayerVtbl;
    if (ball != (CoreObject *)0x0) {
      entry = (const PlayerBallVtbl *)ball->vtable;
      entry->fn((void *)((uintptr_t)ball + entry->adjust), 3);
      self->ball = (void *)0x0;
    }
    if (self->trail != (void *)0x0) {
      ActorBallTrailDtor(self->trail,3);
      self->trail = (void *)0x0;
    }
    ActorDtor(&self->base,0);
    if ((flags & 1) != 0) {
      MemLock();
      MemFree(self,(char *)0x0,0);
      MemUnlock();
    }
  }
  return;
}

