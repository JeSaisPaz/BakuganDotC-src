// bdc 0x088d44d4 GameStageIsOtherPointKind
#include "bdc.h"

/* Returns 0 for kinds 1..3 and 1 otherwise; used by `GameStageSpawnFieldPoints`, which only calls
   it for kinds 1..3 (so its spawn branch never runs). */

s32 GameStageIsOtherPointKind(s32 kind)
{
  if (kind > 0 && kind < 4) {
    return 0;
  }
  return 1;
}
