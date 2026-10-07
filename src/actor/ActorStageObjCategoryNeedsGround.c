// bdc 0x088aac84 ActorStageObjCategoryNeedsGround
#include "bdc.h"

/* Returns 1 when an object of category `category` at `pos` must be collidable: false for categories
   3, 7, 9, 10, 0xc, otherwise whether there is ground below (`ActorStageObjHasGroundBelow`). Also
   used by `ActorStageObjGetIntactBuildingRatio`. */

int ActorStageObjCategoryNeedsGround(float *pos, int category)

{
  int needs;
  float copy[4];

  needs = 0;
  switch (category) {
  case 3:
  case 7:
  case 9:
  case 10:
  case 0xc:
    break;
  default:
    needs = 1;
  }
  if (needs != 0) {
    copy[0] = pos[0];
    copy[1] = pos[1];
    copy[2] = pos[2];
    copy[3] = pos[3];
    if (ActorStageObjHasGroundBelow(copy) == 0) {
      needs = 0;
    }
  }
  return needs;
}
