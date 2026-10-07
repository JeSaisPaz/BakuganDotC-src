// bdc 0x088a6b94 ActorStageObjAttrLandmarkGetRadius
#include "bdc.h"

/* Returns the aura radius for level `level` (`-1` = the object's own level `+0x334`, clamped 0..10)
   from the float `radius` of the rows of `g_stageObjLandmarkHpTable` (× 0.1). */

float ActorStageObjAttrLandmarkGetRadius(ActorStageObjAttrLandmark *self, int level)

{
  if (level == -1) {
    level = self->radiusLevel;
  }
  if (level < 0) {
    level = 0;
  }
  else if (10 < level) {
    level = 10;
  }
  return g_stageObjLandmarkHpTable[level].radius * 0.1f;
}
