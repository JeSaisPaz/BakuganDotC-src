// bdc 0x088d8c98 GameFieldPointListUpdate
#include "bdc.h"

/* Per-frame update of all field points: walks the field point list `g_gameFieldPointList` (created by
   `GameFieldPointInitList`, next pointer at `+4`) and runs `GameFieldPointUpdate` on each point
   (spawns its marker effect when enabled and missing). Does nothing while the list does not exist.
   Called from `GameFieldUpdateWorld`. */

void GameFieldPointListUpdate(void)
{
  GameFieldPoint *point;

  if (g_gameFieldPointList != (GameFieldPoint **)0) {
    for (point = *g_gameFieldPointList; point != (GameFieldPoint *)0; point = point->next) {
      GameFieldPointUpdate(point);
    }
  }
}
