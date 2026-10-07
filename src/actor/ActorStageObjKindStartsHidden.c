// bdc 0x088aaadc ActorStageObjKindStartsHidden
#include "bdc.h"

/* Returns 1 for kinds 0x19 (`TREE01`) and 0x3d (`F1_TOWNTREE01`), which `ActorStageObjBaseInit`
   creates invisible (`+0xbc = 0`) with the default colour. */

int ActorStageObjKindStartsHidden(ActorStageObjBase *self, int kind)
{
  int result = 0;

  if (kind < 0x1a) {
    if (kind >= 0x19) {
      result = 1;
    }
  } else if (kind == 0x3d) {
    result = 1;
  }
  return result;
}
