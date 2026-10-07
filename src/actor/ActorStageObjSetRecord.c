// bdc 0x088ac944 ActorStageObjSetRecord
#include "bdc.h"

/* Stores the layout spawn record `rec` at `+0x154` and clears `+0x1d8`. Called by
   `ActorStageObjRecordSpawn`. */

void ActorStageObjSetRecord(ActorStageObjBase *self, void *rec)

{
  self->record = rec;
  self->recordState = 0;
}
