// bdc 0x088a6754 ActorStageObjWindGeneratorState00
#include "bdc.h"

/* State 0 of the wind generator (table `0x08a83f38`): once the object is destroyed (`+0x281`),
   marks its layout spawn record `+0x154` done (`+0x3a`) a single time (`+0x330`). Created with `bdc
   fn create` (state-table target). */

void ActorStageObjWindGeneratorState00(ActorStageObjWindGenerator *self)

{
  ActorStageObjRecord *rec;

  if (((self->base).dead != '\0') && (self->deathNotified == '\0')) {
    rec = (ActorStageObjRecord *)(self->base).record;
    if (rec != NULL) {
      rec->doneFlags[0] = 1;
    }
    self->deathNotified = '\x01';
  }
  return;
}

