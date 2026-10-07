// bdc 0x088b2178 ActorStageObjRecordIsNotLandmark
#include "bdc.h"

/* Returns 0 when the layout spawn record (`ActorStageObjRecordCtor`) describes a landmark
   (category 5 of its kind `rec+0x32`, `ActorStageObjGetCategory`), else 1;
   `ActorStageObjRecordSpawn` uses it to filter records outside the field scene. */

int ActorStageObjRecordIsNotLandmark(ActorStageObjRecord *rec)
{
  if (ActorStageObjGetCategory(rec->field32[0]) == 5) {
    return 0;
  }
  return 1;
}
