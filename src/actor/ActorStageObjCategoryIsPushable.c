// bdc 0x088a9298 ActorStageObjCategoryIsPushable
#include "bdc.h"

/* Returns 1 for the categories 3 (vehicles/props), 7 (signs) and 0xc (mines) when the field task
   500 is absent (battle), else 0. */

int ActorStageObjCategoryIsPushable(int category)

{
  int result;

  result = 0;
  if ((category == 0xc || category == 7 || category == 3) && CoreTaskExists(500) == 0) {
    result = 1;
  }
  return result;
}
