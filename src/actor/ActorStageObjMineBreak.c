// bdc 0x088a601c ActorStageObjMineBreak
#include "bdc.h"

/* Break virtual of the mine (vtable `0x08af2674` slot 11): when it still has its layout spawn
   record `+0x154`, may drop an item (`ActorStageObjDropItem`), marks the record done and releases
   it (`ActorStageObjReleaseRecord`); then disables the model (`CoreObjectDeferDelete(obj, 0)`).
    */

void ActorStageObjMineBreak(ActorStageObjMine *self)

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

