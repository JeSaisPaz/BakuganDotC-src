// bdc 0x08945f84 UiStaffCreditMainPhase
#include "bdc.h"

/* Main phase (entry 2 of `g_uiStaffCreditPhaseTable`) of the staff-credits screen (task 3004,
   `UiStaffCreditCtor`). Before the fade (sub-state < 2), circle/cross (pad 0x4000/0x2000)
   cancel and stop BGM channel 0 and jump to the fade. Sub-states: 0 roll — updates pictures,
   lines and BGM and advances the roll frame `rollFrame`, to sub-state 1 past frame 0x42fe and to
   the pause (4) when it hits the layout's `pauseFrame` (`g_staffCreditLayout`); 1 — updates the
   BGM and keeps counting until frame 0x43ef or any of buttons 0x4000/0x8000/0x2000/0x1000/0x1/0x8;
   2 — starts a 16-frame fade from `g_staffCreditFadeClear` to `g_staffCreditFadeBlack`;
   3 — once the fade is done closes the shared background (`UiSharedBgClose`), clears
   `g_uiKeepSharedBg` and switches to phase 3; 4 — pauses 150 frames, then resumes the roll one
   frame later. Always ends with `UiStaffCreditSpinEmblem`. */

void UiStaffCreditMainPhase(UiScreen *screen)
{
  UiStaffCredit *self = (UiStaffCredit *)screen;

  if (screen->phaseStep < 2 &&
      ((screen->pad->buttons & 0x4000) != 0 || (screen->pad->buttons & 0x2000) != 0)) {
    SndBgmCancelChannel(0);
    SndBgmQueueStop(0.0f, 0);
    screen->phaseStep = 2;
  }
  switch ((u32)screen->phaseStep) {
  case 0: {
    s32 frame;

    UiStaffCreditUpdatePictures(screen);
    UiStaffCreditUpdateLines(screen);
    UiStaffCreditUpdateBgm(screen);
    frame = (s32)((float)self->rollFrame + 1.0f);
    self->rollFrame = frame;
    if (frame >= 0x42ff) {
      screen->phaseStep = screen->phaseStep + 1;
    }
    if (frame == g_staffCreditLayout.pauseFrame) {
      screen->phaseStep = 4;
    }
    break;
  }
  case 1: {
    s32 frame;
    PadState *pad;

    UiStaffCreditUpdateBgm(screen);
    frame = self->rollFrame;
    pad = screen->pad;
    if (frame >= 0x43ef || (pad->buttons & 0x4000) != 0 || (pad->buttons & 0x8000) != 0 ||
        (pad->buttons & 0x2000) != 0 || (pad->buttons & 0x1000) != 0 ||
        (pad->buttons & 0x1) != 0 || (pad->buttons & 0x8) != 0) {
      screen->phaseStep = screen->phaseStep + 1;
    }
    self->rollFrame = (s32)((float)frame + 1.0f);
    break;
  }
  case 2:
    UiStaffCreditStartFade(16.0f, g_staffCreditFadeClear, g_staffCreditFadeBlack);
    screen->phaseStep = screen->phaseStep + 1;
    break;
  case 3:
    if (UiStaffCreditFadeIsFinished()) {
      UiSharedBgClose();
      g_uiKeepSharedBg = 0;
      screen->phaseStep = 0;
      screen->phase = 3;
    }
    break;
  case 4: {
    s32 timer = (s32)((float)self->pauseTimer + 1.0f);

    self->pauseTimer = timer;
    if (timer >= 0x96) {
      s32 frame = self->rollFrame;

      self->pauseTimer = 0;
      screen->phaseStep = 0;
      self->rollFrame = (s32)((float)frame + 1.0f);
    }
    break;
  }
  default:
    break;
  }
  UiStaffCreditSpinEmblem(screen);
}
