// bdc 0x088a93c4 ActorStageObjKindIsLandmark
#include "bdc.h"

/* Returns 1 for the landmark kinds (`LANDMARK01..10` 0x22..0x2b, `F1_LANDMARK*` 0x40..0x49, `F2_*`
   0x5b..0x64, `F3_*` 0x75..0x7a, `F4_/F5_*` 0x8d..0x98, `F9_*` 0xa1..0xa6), which
   `ActorStageObjGetCategory` maps to category 5. */

int ActorStageObjKindIsLandmark(int kind)

{
  if (kind < 0x65) {
    if (kind < 0x40) {
      if (kind < 0x22) {
        return 0;
      }
      if (0x2b < kind) {
        return 0;
      }
      return 1;
    }
    if (0x49 < kind) {
      if (kind < 0x5b) {
        return 0;
      }
      return 1;
    }
  }
  else {
    if (kind < 0x8d) {
      if (kind < 0x75) {
        return 0;
      }
      if (0x7a < kind) {
        return 0;
      }
      return 1;
    }
    if (kind < 0xa1) {
      if (0x98 < kind) {
        return 0;
      }
      return 1;
    }
    if (0xa6 < kind) {
      return 0;
    }
  }
  return 1;
}

