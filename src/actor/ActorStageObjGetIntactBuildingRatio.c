// bdc 0x088b32cc ActorStageObjGetIntactBuildingRatio
#include "bdc.h"

/* Returns the fraction of buildings still standing on the current stage: walks the stage-object
   layout record list `g_stageObjRecordList` and, for every enabled record (`doneFlags[1]`) whose
   kind (`field32[0]`) maps to a building category (`ActorStageObjGetCategory`,
   `ActorStageObjCategoryIsBuilding`) and that needs ground at its position `pos`
   (`ActorStageObjCategoryNeedsGround`), counts it, and counts it as intact when its destroyed
   flag `doneFlags[0]` is clear. Returns `intact / total`, or 1.0 when no record qualifies
   (total < 1). */

float ActorStageObjGetIntactBuildingRatio(void)
{
  float total = 0.0f;
  float intact = 0.0f;
  ActorStageObjRecord *rec;
  int category;
  float pos[4] __attribute__((aligned(16)));

  if (g_stageObjRecordList != (void **)0x0) {
    for (rec = (ActorStageObjRecord *)*g_stageObjRecordList; rec != (ActorStageObjRecord *)0x0;
         rec = (ActorStageObjRecord *)rec->base.next) {
      if (rec->doneFlags[1] == 0)
        continue;
      category = ActorStageObjGetCategory(rec->field32[0]);
      if (ActorStageObjCategoryIsBuilding(category) == 0)
        continue;
      pos[0] = rec->pos[0];
      pos[1] = rec->pos[1];
      pos[2] = rec->pos[2];
      pos[3] = rec->pos[3];
      if (ActorStageObjCategoryNeedsGround(pos, category) == 0)
        continue;
      if (rec->doneFlags[0] == 0)
        intact = intact + 1.0f;
      total = total + 1.0f;
    }
  }
  if (total < 1.0f)
    return 1.0f;
  return intact / total;
}
