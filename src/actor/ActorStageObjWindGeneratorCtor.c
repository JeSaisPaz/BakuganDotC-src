// bdc 0x088a6564 ActorStageObjWindGeneratorCtor
#include "bdc.h"

/* Constructor of the wind-generator stage object (category 0xd: kind 0xb5
   `GIMMICK_22_WINDGENERATOR`, 0x340 bytes): `ActorStageObjBaseCtor`, vtable `0x08af2714`,
   visible, alpha 1, kind stored at `+0x328`. */

void *ActorStageObjWindGeneratorCtor(ActorStageObjWindGenerator *self, int kind, float *pos)

{
  ActorStageObjBaseCtor(&self->base,kind,pos);
  (self->base).base.base.vtable = g_actorStageObjWindGeneratorVtbl;
  self->reserved320[0] = 0;
  self->reserved320[1] = 0;
  self->state = 0;
  (self->base).base.lighting = '\x01';
  self->deathNotified = '\0';
  (self->base).fade = 1.0f;
  self->kind = kind;
  return self;
}

