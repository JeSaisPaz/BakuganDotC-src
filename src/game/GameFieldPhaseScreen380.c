// bdc 0x088c2e5c GameFieldPhaseScreen380
#include "bdc.h"

/* Phase 6 of the field task (id 500, `GameFieldCtor`): returns to phase 1 once task 380
   (`UiEmptyScreen380Ctor`) is gone. */

void GameFieldPhaseScreen380(CoreTask *task)

{
  GameFieldTask *field = (GameFieldTask *)task;

  if (CoreTaskExists(0x17c) == 0) {
    field->phase = 1;
  }
}
