// bdc 0x08930f84 UiBakuganSelectMainPhase
#include "bdc.h"

/* Phase 2 (main) of `UiBakuganSelect` (phase table `0x08a9c340`), a
   `phaseStep` (`base.phaseStep`) state machine (jump table `0x08a9c660`):
   0 starts the cell blink (sprite 0x12) and scroll loop (sprites 0x13/0x14) records and the frame
   tweens (`UiBakuganSelectTweenFrame`); 1 waits for them, then loads the pedestal;
   2 starts the gauges, name panel and the six open tweens, loads the model (ambient alpha 1);
   3 waits until all six report done, then starts the arrows and resets the cursor;
   4 is the input loop: cursor pulse, `UiBakuganSelectAnimateFocus`, `UiPulseStep`;
   decide (`UiBakuganSelectCheckDecide`) 1 → SE 0, mark seen, flash, `cancelled` = 0, step 12;
   decide 2 → SE 3 only; Circle (`pressed` 0x2000) → SE 2, `cancelled` = 1, step 6;
   a cursor move (`UiBakuganSelectMoveCursor` == 1) → SE 1, apply cursor, attribute icons, step 5;
   5 reloads the model when `currentChanged`, back to 4; 6–7 start and wait for the close tweens,
   then release model and pedestal; 8–9 close the frame; 10–11 fade to black over 16 frames, then
   step 14; 12 waits for the decide flash, sets `decideHold` = 16; 13 counts it down, then step 6;
   any other step sets the menu result (`UiBakuganSelectSetResult`), phase 3, step 0.
   Every frame then updates cell blink, scroll loop, gauges, name-panel pop tween and arrows. */

void UiBakuganSelectMainPhase(UiBakuganSelect *self)

{
  GfxSprite **sprites;
  GfxFader *fader;
  u8 done;
  u8 decide;

  switch (self->base.phaseStep) {
  case 0:
    sprites = (GfxSprite **)self->base.data;
    UiCellBlinkInit(1, sprites[0x12], &self->cellBlink);
    sprites = (GfxSprite **)self->base.data;
    UiScrollLoopInit(1, sprites[0x13], sprites[0x14], &self->scrollLoop);
    UiBakuganSelectTweenFrame(self, 0);
    self->base.phaseStep++;
    break;
  case 1:
    if ((u8)UiBakuganSelectFrameDone(self, 0) == 1) {
      UiBakuganSelectLoadPedestal(self);
      self->base.phaseStep++;
    }
    break;
  case 2:
    UiBakuganSelectStartGaugeAnim(self, 1, (u8)self->cursor);
    UiBakuganSelectShowNamePanel(self, 1, 0);
    UiBakuganSelectTweenHelpPanel(self, 0);
    UiBakuganSelectTweenStatBarsA(self, 0);
    UiBakuganSelectTweenStatBarsB(self, 0);
    UiBakuganSelectTweenHeaderSprites(self, 0);
    UiBakuganSelectTweenGrid(self, 0);
    UiBakuganSelectLoadModel(self);
    ((GfxModel *)self->model)->ambient[3] = 1.0f;
    UiBakuganSelectTweenTypeIcons(self, 0);
    self->base.phaseStep++;
    break;
  case 3:
    done = UiBakuganSelectHelpPanelDone(self, 0);
    done += UiBakuganSelectStatBarsADone(self, 0);
    done += UiBakuganSelectStatBarsBDone(self, 0);
    done += UiBakuganSelectHeaderDone(self, 0);
    done += UiBakuganSelectGridDone(self, 0);
    done += UiBakuganSelectTypeIconsDone(self, 0);
    if (done == 6) {
      UiBakuganSelectStartArrows(self, 1);
      UiBakuganSelectResetCursor(self);
      self->base.phaseStep++;
    }
    break;
  case 4:
    UiBakuganSelectStartCursorPulse(self);
    UiBakuganSelectAnimateFocus(self);
    UiPulseStep(((GfxSprite **)self->base.data)[0x84], (UiPulse *)&self->tweens[0x84]);
    decide = (u8)UiBakuganSelectCheckDecide(self);
    if (decide == 0) {
      if ((self->base.pad->pressed & 0x2000) != 0) {
        if (SndHasManager()) {
          SndManagerPlay(SndGetManager(), 2, 0, 0);
        }
        self->cancelled = 1;
        UiBakuganSelectResetCursor(self);
        self->base.phaseStep = 6;
      }
      else if (UiBakuganSelectMoveCursor(self) == 1) {
        if (SndHasManager()) {
          SndManagerPlay(SndGetManager(), 1, 0, 0);
        }
        UiBakuganSelectResetCursor(self);
        UiBakuganSelectApplyCursor(self);
        UiBakuganSelectSetAttributeIcons(self);
        self->base.phaseStep = 5;
      }
    }
    else if (decide == 1) {
      if (SndHasManager()) {
        SndManagerPlay(SndGetManager(), 0, 0, 0);
      }
      UiBakuganSelectMarkSeen(self);
      UiBakuganSelectResetCursor(self);
      UiBakuganSelectFlashSelection(self);
      self->cancelled = 0;
      self->base.phaseStep = 12;
    }
    else {
      if (SndHasManager()) {
        SndManagerPlay(SndGetManager(), 3, 0, 0);
      }
    }
    break;
  case 5:
    if (self->currentChanged != 0) {
      UiBakuganSelectLoadModel(self);
      ((GfxModel *)self->model)->ambient[3] = 1.0f;
    }
    self->base.phaseStep = 4;
    break;
  case 6:
    UiBakuganSelectStartArrows(self, 0);
    UiBakuganSelectShowCurrentMark(self, false);
    UiBakuganSelectHideCursors(self);
    UiBakuganSelectShowNamePanel(self, 1, 1);
    UiBakuganSelectTweenHelpPanel(self, 1);
    UiBakuganSelectTweenStatBarsA(self, 1);
    UiBakuganSelectTweenStatBarsB(self, 1);
    UiBakuganSelectTweenHeaderSprites(self, 1);
    UiBakuganSelectTweenGrid(self, 1);
    UiBakuganSelectTweenTypeIcons(self, 1);
    self->base.phaseStep++;
    break;
  case 7:
    done = UiBakuganSelectHelpPanelDone(self, 1);
    done += UiBakuganSelectStatBarsADone(self, 1);
    done += UiBakuganSelectStatBarsBDone(self, 1);
    done += UiBakuganSelectHeaderDone(self, 1);
    done += UiBakuganSelectGridDone(self, 1);
    done += UiBakuganSelectTypeIconsDone(self, 1);
    if (done == 6) {
      UiBakuganSelectReleaseModel(self);
      UiBakuganSelectReleasePedestal(self);
      self->base.phaseStep++;
    }
    break;
  case 8:
    UiBakuganSelectTweenFrame(self, 1);
    self->base.phaseStep++;
    break;
  case 9:
    if ((u8)UiBakuganSelectFrameDone(self, 1) == 1) {
      self->base.phaseStep++;
    }
    break;
  case 10:
    fader = GfxGetActiveFader();
    fader->start[0] = 0.0f;
    fader->start[1] = 0.0f;
    fader->start[2] = 0.0f;
    fader->start[3] = 0.0f;
    fader = GfxGetActiveFader();
    fader->end[0] = 0.0f;
    fader->end[1] = 0.0f;
    fader->end[2] = 0.0f;
    fader->end[3] = 1.0f;
    GfxFaderStart(GfxGetActiveFader(), 16);
    self->base.phaseStep++;
    break;
  case 11:
    if (GfxFaderIsFinished(GfxGetActiveFader())) {
      self->base.phaseStep = 14;
    }
    break;
  case 12:
    if (UiBakuganSelectFlashDone() == 1) {
      self->decideHold = 16;
      self->base.phaseStep++;
    }
    break;
  case 13:
    if (self->decideHold != 0) {
      self->decideHold--;
    }
    else {
      self->base.phaseStep = 6;
    }
    break;
  default:
    UiBakuganSelectSetResult(self);
    self->base.phase = 3;
    self->base.phaseStep = 0;
    break;
  }
  UiCellBlinkUpdate(&self->cellBlink);
  UiScrollLoopUpdate(&self->scrollLoop);
  UiBakuganSelectUpdateGauges(self);
  UiBakuganSelectUpdatePopTween(self);
  UiBakuganSelectUpdateArrows(self);
  return;
}
