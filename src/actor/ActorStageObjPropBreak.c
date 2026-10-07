// bdc 0x088b0f30 ActorStageObjPropBreak
#include "bdc.h"

/* Break virtual of the knock-over prop (vtable `0x08af2ae4` slot 11): while it still has its layout
   record `+0x154`, may drop an item (`ActorStageObjDropItem`), marks the record done and releases
   it; then schedules deletion (`CoreObjectDeferDelete`). */

void ActorStageObjPropBreak(ActorStageObjProp *self)

{
  if ((self->base).record != (void *)0x0) {
    ActorStageObjDropItem(&self->base);
    ((ActorStageObjRecord *)(self->base).record)->doneFlags[0] = 1;
    ActorStageObjReleaseRecord((self->base).record);
    (self->base).record = (void *)0x0;
  }
  CoreObjectDeferDelete((CoreObject *)self,0);
  return;
}

