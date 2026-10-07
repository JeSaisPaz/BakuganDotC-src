// bdc 0x088a6bec ActorStageObjAttrLandmarkGetMaxHp
#include "bdc.h"

/* Returns the HP for level `level` (`-1` = the object's `+0x330`, clamped 0..10) from the 16-byte
   rows at `0x08a85558` (word 1: 0, 100, 250, 250, 750, 500, 1200, 1000, 1600, 1800, 2000) as a
   float. */

float ActorStageObjAttrLandmarkGetMaxHp(ActorStageObjAttrLandmark *self, int level)

{
  if (level == -1) {
    level = self->hpLevel;
  }
  if (level < 0) {
    level = 0;
  }
  else if (10 < level) {
    level = 10;
  }
  return (float)g_stageObjLandmarkHpTable[level].maxHp;
}

