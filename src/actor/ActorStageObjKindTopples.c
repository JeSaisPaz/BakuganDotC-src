// bdc 0x088acd60 ActorStageObjKindTopples
#include "bdc.h"

/* Returns 1 for the kinds that fall over as a whole when destroyed: signs and billboards
   (0x1a..0x1d, 0x56, 0x5a), `F1_BRIDGE01` 0x2d, street lights/signals 0x37/0x3b (via the table
   `0x08a84aa0`), 0x6c, 0x6d, 0x72 and `F4_WALL01` 0x8b. Used by
   `ActorStageObjState08Update`, `ActorStageObjPropState02Knocked`, `ActorStageObjPropState03Topple`. */

int ActorStageObjKindTopples(ActorStageObjBase *self)
{
  int kind = self->kind;

  if (kind < 0x6c) {
    if (kind >= 0x3c) {
      if (kind >= 0x57) {
        return kind == 0x5a;
      }
      return kind >= 0x56;
    }
    if (kind < 0x1e) {
      return kind >= 0x1a;
    }
    if (kind < 0x2d) {
      return 0;
    }
    switch (kind - 0x2d) {
    case 0:
    case 10:
    case 14:
      return 1;
    default:
      return 0;
    }
  }
  if (kind < 0x73) {
    if (kind >= 0x6e) {
      return kind >= 0x72;
    }
    return 1;
  }
  return kind == 0x8b;
}
