// bdc 0x088ace30 ActorStageObjIsBillboard
#include "bdc.h"

/* Returns 1 for the billboard kinds 0x1a (`F0_BILLBOARD01`), 0x56/0x57 (`F2_BILLBOARD01/02`) and
   0x6d (`F3_BILLBOARD01`), else 0. Used by `ActorStageObjState08Update` and `ActorStageObjPropState03Topple`. */

int ActorStageObjIsBillboard(ActorStageObjBase *self)

{
  int kind;

  kind = self->kind;
  if (kind < 0x56) {
    return kind == 0x1a;
  }
  if (kind < 0x6d) {
    return kind < 0x58;
  }
  return kind < 0x6e;
}
