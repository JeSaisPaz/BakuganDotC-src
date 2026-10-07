// bdc 0x088f3968 GameEventPropTweenCtor
#include "bdc.h"

/* Constructor of the prop tween action (vtable `0x08af437c`, 0xc bytes): prop record `rec`,
   duration `frames`, frame 0, kind, slot. */

GameEventPropTween *GameEventPropTweenCtor(GameEventPropTween *self, GameEventPropRecord *rec, u8 frames, u8 kind, u8 target)

{
  (self->base).vtbl = g_gameEventPropTweenVtbl;
  self->rec = rec;
  self->frames = frames;
  self->frame = '\0';
  self->kind = kind;
  self->target = target;
  return self;
}

