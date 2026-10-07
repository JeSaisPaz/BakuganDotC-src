// bdc 0x088a9330 ActorStageObjKindIsAttrLandmark
#include "bdc.h"

/* Returns 1 for kinds 0xbb..0xcc (the attribute landmarks `FIRE_LV1`..`DARK_LV3`,
   `ActorStageObjAttrLandmarkCtor`), else 0. */

int ActorStageObjKindIsAttrLandmark(int kind)

{
  if (kind >= 0xbb && kind <= 0xcc) {
    return 1;
  }
  return 0;
}

