// bdc 0x088b0940 ActorStageObjScreenBreak
#include "bdc.h"

/* Break virtual of the screen (vtable `0x08af2a44` slot 11): marks its layout record `+0x154` done
   and releases it (`ActorStageObjReleaseRecord`) and schedules deletion
   (`CoreObjectDeferDelete`). */

void ActorStageObjScreenBreak(ActorStageObjScreen *self)

{
  if ((self->base).record != (void *)0x0) {
    ((ActorStageObjRecord *)(self->base).record)->doneFlags[0] = 1;
    ActorStageObjReleaseRecord((self->base).record);
    (self->base).record = (void *)0x0;
  }
  CoreObjectDeferDelete((CoreObject *)self,0);
  return;
}

