// bdc 0x088f2874 GameEventFadeActionCtor
#include "bdc.h"

/* Constructor of the screen-fade action (vtable `0x08af42d4`, 0xc bytes): target fade record `rec`,
   duration `frames`, frame 0, flags. */

GameEventFadeAction *GameEventFadeActionCtor(GameEventFadeAction *self, u8 skip, GameEventFadeRecord *rec, u8 frames, u8 target)

{
  (self->base).vtbl = g_gameEventFadeActionVtbl;
  self->rec = rec;
  self->frames = frames;
  self->frame = '\0';
  self->target = target;
  self->skip = skip;
  return self;
}

