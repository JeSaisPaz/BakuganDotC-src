// bdc 0x08948fa0 UiBattleRecordFinishPhase
#include "bdc.h"

/* Phase 1 of `UiBattleRecord`, one step per call (`base.phaseStep`):
   step 0 swaps shared animation slot 1 to `"main_finish.fab"` (two `GfxFabUpdate` ticks),
   flips the title plate closed (`UiTitlePlateInit`) and resets `animFrame`; step 1 runs the
   menu close animation (`UiBattleRecordAnimateMenu``(…, 1)`) and, once it is done, starts a
   12-frame fade from `g_battleRecordFadeColorClear` to `g_battleRecordFadeColorOpaque`;
   step 2 waits for the fade, closes the shared background (`UiSharedBgClose`) and clears
   `g_uiKeepSharedBg`; step 3 (and any other step) waits until task 320 is gone, then reports
   menu result 0, requests the close and sets phase 6. */

void UiBattleRecordFinishPhase(UiBattleRecord *self)
{
  switch (self->base.phaseStep) {
  case 0:
    UiSharedAnimRelease(self, 1);
    UiSharedAnimStart(20.0f, 0.0f, 0.0f, self, (void *)"main_finish.fab", 1, 0);
    GfxFabUpdate(g_uiSharedAnims[1]);
    GfxFabUpdate(g_uiSharedAnims[1]);
    UiTitlePlateInit(1, ((GfxSprite **)self->base.data)[15]);
    self->animFrame = 0;
    self->base.phaseStep = 1;
    return;
  case 1:
    if (UiBattleRecordAnimateMenu(self, 1) != 0) {
      UiBattleRecordStartFade(12.0f, g_battleRecordFadeColorClear, g_battleRecordFadeColorOpaque);
      self->base.phaseStep = 2;
    }
    return;
  case 2:
    if (UiBattleRecordFadeIsFinished()) {
      UiSharedBgClose();
      g_uiKeepSharedBg = 0;
      self->base.phaseStep = 3;
    }
    return;
  default:
    if (CoreTaskExists(0x140) == 0) {
      UiSetMenuResult(&self->base, 0);
      self->base.closeRequested = 1;
      self->base.phaseStep = 0;
      self->base.phase = 6;
    }
    return;
  }
}
