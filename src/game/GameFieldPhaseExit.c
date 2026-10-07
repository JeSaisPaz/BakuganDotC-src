// bdc 0x088c2cac GameFieldPhaseExit
#include "bdc.h"

/* Phase 3 of the field task (id 500, `GameFieldCtor`): step machine on the sub-state `+0x61c`:
   0 stops all sound (`GameFieldStopAllSound`); 1 waits for BGM player 1 to stop, then releases
   the voice pack (`SndVoicePacRelease`) until it is idle; 2 waits for the voice pack to be idle;
   3 resets the clear colour to `g_colorBlack`; 4 waits for the sound group loader, then hands
   the next request `+0x620` to script variable 3 and removes the task (`CoreTaskRemove`). */

void GameFieldPhaseExit(CoreTask *task)

{
  GameFieldTask *field = (GameFieldTask *)task;
  GfxDisplay *display;

  switch (field->subState) {
  case 0:
    GameFieldStopAllSound();
    field->subState++;
    break;
  case 1:
    if (SndBgmPlayerExists(1) && !SndBgmPlayerIsStopped(SndBgmPlayerGet(1))) {
      break;
    }
    if (SndVoicePacIsIdle()) {
      field->subState++;
    }
    else {
      SndVoicePacRelease();
    }
    break;
  case 2:
    if (SndVoicePacIsIdle()) {
      field->subState++;
    }
    break;
  case 3:
    display = g_gfxDisplay;
    display->clearColor[0] = g_colorBlack.x;
    display->clearColor[1] = g_colorBlack.y;
    display->clearColor[2] = g_colorBlack.z;
    display->clearColor[3] = g_colorBlack.w;
    field->subState++;
    break;
  case 4:
    if (SndIsGroupLoaderIdle()) {
      g_scriptGlobalVars[3] = field->nextRequest;
      CoreTaskRemove(task, true);
    }
    break;
  }
}
