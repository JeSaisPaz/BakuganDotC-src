// bdc 0x088beda4 GameFieldOpenPauseMenu
#include "bdc.h"

/* Opens the pause menu from the field task (id 500): creates `UiPauseCtor` (task 410 = 0x19a)
   with `CoreTaskCreate`, initialises it with `mode` (`UiPauseSetMenuKind`; mode 6 with a net-play
   session also builds a pad object), records `mode` at `+0x6a4`, switches the field to phase 2
   (`GameFieldPhasePause`, `+0x618 = 2`) and pauses the player (`GameFieldSetPlayerPaused` 1).
    */

void GameFieldOpenPauseMenu(void *taskPtr, s32 mode)
{
  GameFieldTask *task = taskPtr;
  UiPause *self;

  self = (UiPause *)CoreTaskCreate(0x19a, 100);
  if (self != NULL) {
    UiPauseSetMenuKind(self, mode);
  }
  task->pauseMode = mode;
  task->phase = 2;
  GameFieldSetPlayerPaused((CoreTask *)task, true);
}
