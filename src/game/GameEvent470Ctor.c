// bdc 0x088efa3c GameEvent470Ctor
#include "bdc.h"

/* Constructor of the field event task class for task id 470 (0x1d6), built by `CoreTaskNewById`
   (object size 0x2d8): runs the base `GameEventCtor`, installs vtable `0x08af425c`, clears the
   derived fields (`+0x284..+0x28c`, flag bits at `+0x2d6`, a 0x40-byte block at `+0x292`) and
   copies the 16-byte default block `0x08a99250` to `+0x274`. */

GameEvent470 *GameEvent470Ctor(GameEvent470 *self)

{
  GameEventCtor(&self->base);
  self->flags470 = self->flags470 & 0xfe;
  (self->base).base.vtable = g_gameEvent470Vtbl;
  self->actors = (GameEventActorRecord *)0x0;
  self->loadValue = 0;
  self->testFlag = 0;
  self->motionIndex = 0;
  self->talkA = '\0';
  self->talkB = '\0';
  self->talkPartner = '\0';
  self->flags470 = self->flags470 & 0xfd;
  self->flags470 = self->flags470 & 0xfb;
  memcpy(&self->turnPending, g_gameEvent470TurnDefaults, 0x10);
  memset(self->actorMap,0,0x40);
  return self;
}

