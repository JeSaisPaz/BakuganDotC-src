// bdc 0x088f3360 GameEventActorTweenCtor
#include "bdc.h"

/* Constructor of the event-actor tween action (vtable `0x08af4344`, 0xc bytes): record `rec`,
   duration `frames`, frame 0, kind, actor index. */

GameEventActorTween *GameEventActorTweenCtor(GameEventActorTween *self, GameEventActorRecord *rec, u8 frames, u8 kind, u8 target)

{
  (self->base).vtbl = g_gameEventActorTweenVtbl;
  self->rec = rec;
  self->frames = frames;
  self->frame = '\0';
  self->kind = kind;
  self->target = target;
  return self;
}

