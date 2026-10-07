// bdc 0x088c2de4 GameFieldPhaseRepair
#include "bdc.h"

/* Phase 4 of the field task (id 500, `GameFieldCtor`): returns to phase 1 once the repair screen
   (`UiRepairCtor`, task 430) is gone. */

void GameFieldPhaseRepair(CoreTask *task)
{
  if (CoreTaskExists(0x1ae) == 0) {
    ((GameFieldTask *)task)->phase = 1;
  }
}
