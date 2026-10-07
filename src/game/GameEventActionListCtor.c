// bdc 0x088eab90 GameEventActionListCtor
#include "bdc.h"

/* Constructor of the event action list task (vtable `0x08af41b4`, 0x54 bytes, `ev+0x3c`):
   `CoreTaskInit`, clears the state `+0x50`, the count `+0x51` and the 16 action slots
   `+0x10..+0x4c`. Built by `GameEventInit`. */

GameEventActionList *GameEventActionListCtor(GameEventActionList *self)

{
  CoreTaskInit(&self->base);
  (self->base).vtable = g_gameEventActionListVtbl;
  self->state = '\0';
  self->count = '\0';
  memset(self->actions,0,0x40);
  return self;
}

