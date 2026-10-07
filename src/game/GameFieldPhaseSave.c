// bdc 0x088c2e94 GameFieldPhaseSave
#include "bdc.h"

/* Phase 7 of the field task (id 500, `GameFieldCtor`): once the save task (`SaveSaveTaskCtor`,
   10020) is gone, re-enables the world-map task `+0x6c8`, clears profile flag `0x40000000`, reopens
   the pause menu (mode 5 on area 8, else 4, `GameFieldOpenPauseMenu`) with the cursor on its quit
   entry, fades the active fader from its current colour to transparent if it is visible
   (`GfxFaderStart` 20 frames), and when the camera mode `+0x2cc` is 4 restarts the stage BGM
   (`GameFieldPlayStageBgm`) and sets the sub-state `+0x61c` to 2. */

void GameFieldPhaseSave(CoreTask *task)

{
  GameFieldTask *field = (GameFieldTask *)task;
  UiPause *pause;
  GfxFader *dst;
  GfxFader *src;
  GfxFader *fader;

  if (CoreTaskExists(0x2724) != 0) {
    return;
  }
  if (field->worldMapTask != NULL) {
    CoreTaskClearFlags(field->worldMapTask, 1);
  }
  SaveProfileClearFlags(SaveGetProfile(), 0x40000000);
  if (g_gameEventFlags[0] == 8) {
    GameFieldOpenPauseMenu(task, 5);
  }
  else {
    GameFieldOpenPauseMenu(task, 4);
  }
  pause = (UiPause *)CoreTaskFind(0x19a);
  if (pause != NULL) {
    UiPauseSelectQuitEntry(pause);
  }
  if (!(GfxGetActiveFader()->color[3] <= 0.0f)) {
    dst = GfxGetActiveFader();
    src = GfxGetActiveFader();
    dst->start[0] = src->color[0];
    dst->start[1] = src->color[1];
    dst->start[2] = src->color[2];
    dst->start[3] = src->color[3];
    fader = GfxGetActiveFader();
    fader->end[0] = 0.0f;
    fader->end[1] = 0.0f;
    fader->end[2] = 0.0f;
    fader->end[3] = 0.0f;
    GfxFaderStart(GfxGetActiveFader(), 20);
  }
  if (field->cameraMode == 4) {
    GameFieldPlayStageBgm();
    field->subState = 2;
  }
}
