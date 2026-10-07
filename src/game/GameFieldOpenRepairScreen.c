// bdc 0x088bee08 GameFieldOpenRepairScreen
#include "bdc.h"

/* Opens the repair screen from the field task: creates `UiRepairCtor` (task 430 = 0x1ae), stores
   `value` in its `value` field and switches the field to phase 4 (`GameFieldPhaseRepair`, which waits
   for the screen to close). */

void GameFieldOpenRepairScreen(void *task, s32 value)
{
  UiRepair *repair = (UiRepair *)CoreTaskCreate(0x1ae, 100);

  if (repair != (UiRepair *)0) {
    repair->value = value;
  }
  ((GameFieldTask *)task)->phase = 4;
}
