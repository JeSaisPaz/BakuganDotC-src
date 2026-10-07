// bdc 0x088b2890 ActorStageObjRecordCtor
#include "bdc.h"

/* Constructor of a stage layout spawn record (`CoreObject`-derived, 0x40 bytes, vtable
   `0x08af6db0`): clears the live-object count `+0x18`, sets the type `+0x20`.. fields: `+0x30` =
   -1, `+0x32..+0x38` = 0 and the done flags `+0x3a/+0x3b` = 0. */

void *ActorStageObjRecordCtor(void *rec)

{
  ActorStageObjRecord *self = (ActorStageObjRecord *)rec;

  CoreObjectInit(&self->base,NULL);
  self->base.vtable = &g_gameStageLayoutObjVtbl;
  self->liveCount = 0;
  self->field30 = -1;
  self->field32[0] = 0;
  self->field32[1] = 0;
  self->field32[2] = 0;
  self->field32[3] = 0;
  self->doneFlags[0] = 0;
  self->doneFlags[1] = 0;
  return rec;
}
