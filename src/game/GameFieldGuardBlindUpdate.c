// bdc 0x088ea308 GameFieldGuardBlindUpdate
#include "bdc.h"

/* Per-frame update of the guard-blind timer (from `GameFieldPhaseMain`): unless the field mode
   (`GameFieldFindTask()+0x61c`) is 5 or 0x1e, runs state 0 (`GameFieldGuardBlindStepHidden`) or 1
   (`GameFieldGuardBlindStepBlink`). */

void GameFieldGuardBlindUpdate(s32 *blind)
{
  u32 mode = ((GameFieldTask *)GameFieldFindTask())->subState;

  if (mode == 5 || mode == 0x1e) {
    return;
  }
  if (*blind > 0) {
    if (*blind < 2) {
      GameFieldGuardBlindStepBlink(blind);
    }
  } else if (*blind >= 0) {
    GameFieldGuardBlindStepHidden(blind);
  }
}
