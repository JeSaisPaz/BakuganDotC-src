// bdc 0x088a92f0 ActorStageObjCategoryIsBuilding
#include "bdc.h"

/* Returns 1 for categories 0..2 and 4 (buildings, cranes, warehouses, chimneys/towers), else 0.
   Called by `ActorStageObjGetIntactBuildingRatio`. */

int ActorStageObjCategoryIsBuilding(int category)
{
  int result = 0;

  if (category >= 0) {
    if (category < 4) {
      if (category < 3) {
        result = 1;
      }
    } else if (category < 5) {
      result = 1;
    }
  }
  return result;
}
