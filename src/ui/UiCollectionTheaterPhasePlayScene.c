// bdc 0x089882b0 UiCollectionTheaterPhasePlayScene
#include "bdc.h"

/* Phase 3 of `UiCollectionTheater`: keeps the shared background, opens
   the scene player (task 520, `maybe_Task520Ctor`) with the event id of the selected slot
   (`UiCollectionTheaterMapSceneId`), waits for it to end, marks the scene viewed
   (`UiCollectionTheaterMarkSceneSeen`), restores frame mode 0 and returns to the main phase at
   step 0xd (fade back in). */

void UiCollectionTheaterPhasePlayScene(UiScreen *screen)

{
  UiCollectionTheater *self = (UiCollectionTheater *)screen;
  UiScenePlayerTask *task;
  s32 step = screen->phaseStep;

  if (step < 1) {
    if (-1 < step) {
      g_uiKeepSharedBg = 1;
      task = (UiScenePlayerTask *)CoreTaskCreateDefault(0x208, (void *)(intptr_t)4);
      task->eventId = UiCollectionTheaterMapSceneId(
          0, self->sceneId[self->page * 6 + self->cursor]);
      screen->phaseStep = screen->phaseStep + 1;
      return;
    }
  }
  else if (step < 2) {
    if (CoreTaskExists(0x208) != 0) {
      return;
    }
    screen->phaseStep = 2;
    return;
  }
  UiScreenSetFrameMode(&screen->base, 0);
  UiCollectionTheaterMarkSceneSeen(screen);
  screen->phase = 2;
  screen->phaseStep = 0xd;
  return;
}
