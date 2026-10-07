// bdc 0x0896efc4 UiCardEquipPhaseMain
#include "bdc.h"

/* Main phase (entry 2 of the phase table `0x08a9d900`) of the ability-card loadout screen
   `UiCardEquip` (task 303). Step 0 builds the card-name/help text printers and
   initialises every sprite group, step 1 runs the open zoom (`UiCardEquipUpdateOpenZoom` +
   `UiCardEquipUpdateFade`) and places the cursors, step 2 handles the top tab row (Up/Down moves,
   Cross confirms, Circle cancels: sets the cancel flag `+0x29bc` and closes), steps 3/4 close the
   screen, step 5 plays the confirm press animation (`UiCardEquipUpdatePress`) then runs the
   chosen tab (`UiCardEquipConfirmTab`), step 6 is the card grid of the selected Bakugan (Cross
   toggles a card, at most two active per Bakugan; Down leaves to the G-power gauge), step 7 is the
   gauge row (Left/Right ±10 between 50 and 150 via `UiCardEquipAdjustGauge`). Any later step
   saves the loadout (`UiCardEquipSaveLoadout`) and moves to the close phase 4. Every frame ends
   with `UiCardEquipUpdatePointerArrows`. */

void UiCardEquipPhaseMain(UiCardEquip *self)
{
  u8 zoomDone;
  u8 result;

  switch (self->base.phaseStep) {
  case 0:
    UiCardEquipCreateNamePrinter(self);
    UiCardEquipCreateHelpPrinter(self);
    UiCardEquipInitTextAnchors(self);
    UiCardEquipInitBgPanels(self);
    UiCardEquipInitBakuganIcons(self);
    UiCardEquipInitTabButtons(self);
    UiCardEquipInitTabLabels(self);
    UiCardEquipInitGroup6Nop(self);
    UiCardEquipInitDecorations(self);
    UiCardEquipInitGaugeArrows(self);
    UiCardEquipInitGaugeLimitMarks(self);
    UiCardEquipRefreshCardIcons(self);
    UiCardEquipRefreshGaugeDigits(self);
    UiCardEquipInitGaugeBars(self);
    UiCardEquipInitMirroredSprites(self);
    UiCardEquipDimDisabledTabs(self);
    UiCardEquipInitLargeExtras(self);
    UiCardEquipStartFade(self, 0);
    UiCardEquipStartOpenZoom(self, 0);
    self->base.phaseStep++;
    break;
  case 1:
    zoomDone = UiCardEquipUpdateOpenZoom(self, 0);
    if ((u8)(zoomDone + UiCardEquipUpdateFade(self, 0)) == 2) {
      UiCardEquipRefreshActiveMarksB(self);
      UiCardEquipRefreshCardMarkers(self);
      UiCardEquipRefreshActiveMarksA(self);
      UiCardEquipRefreshEmptySlots(self);
      UiCardEquipMeasureCardMarkers(self);
      UiCardEquipSetRowFocus(self, (u8)self->row, 1);
      UiCardEquipEnablePointerArrows(self, 1);
      self->base.phaseStep++;
    }
    break;
  case 2: /* tab row */
    UiCardEquipPulseCursor(self);
    UiCardEquipZoomCursorItem(self, (u8)self->row);
    UiCardEquipAnimateTabButton(self);
    UiPulseStep(((GfxSprite **)self->base.data)[self->highlightSprite],
                &self->pulses[self->highlightSprite]);
    result = (u8)UiCardEquipHandleConfirm(self);
    if (result == 0) {
      if ((self->base.pad->pressed & 0x2000) != 0) { /* Circle: cancel and close */
        if (SndHasManager()) {
          SndManagerPlay(SndGetManager(), 2, 0, 0);
        }
        self->cancelled = 1;
        self->base.phaseStep = 3;
      } else if (UiCardEquipMoveCursor(self) == 1) {
        if (SndHasManager()) {
          SndManagerPlay(SndGetManager(), 1, 0, 0);
        }
        UiCardEquipSetRowFocus(self, (u8)self->row, 1);
      }
    } else if (result == 1) {
      if (SndHasManager()) {
        SndManagerPlay(SndGetManager(), 0, 0, 0);
      }
      UiCardEquipSetRowFocus(self, (u8)self->row, 1);
      UiCardEquipStartPress(self);
      self->cancelled = 0;
      self->base.phaseStep = 5;
    } else {
      if (SndHasManager()) {
        SndManagerPlay(SndGetManager(), 3, 0, 0);
      }
    }
    break;
  case 3:
    UiCardEquipEnablePointerArrows(self, 0);
    UiCardEquipSetRowFocus(self, (u8)self->row, 0);
    UiCardEquipStartFade(self, 1);
    UiCardEquipStartOpenZoom(self, 1);
    self->base.phaseStep++;
    break;
  case 4:
    zoomDone = UiCardEquipUpdateOpenZoom(self, 1);
    if ((u8)(zoomDone + UiCardEquipUpdateFade(self, 1)) == 2) {
      self->base.phaseStep = 8;
    }
    break;
  case 5:
    if (UiCardEquipUpdatePress(self) == 1) {
      UiCardEquipConfirmTab(self);
    }
    break;
  case 6: /* card grid */
    UiCardEquipPulseCursor(self);
    UiCardEquipZoomCursorItem(self, (u8)self->row);
    UiPulseStep(((GfxSprite **)self->base.data)[self->highlightSprite],
                &self->pulses[self->highlightSprite]);
    result = (u8)UiCardEquipHandleConfirm(self);
    if (result == 0) {
      if ((self->base.pad->pressed & 0x2000) != 0) { /* Circle: back to the tab row */
        if (SndHasManager()) {
          SndManagerPlay(SndGetManager(), 2, 0, 0);
        }
        UiCardEquipSetRowFocus(self, (u8)self->row, 0);
        self->row = 0;
        UiCardEquipSetRowFocus(self, (u8)self->row, 1);
        UiCardEquipFollowCardMarkers(self);
        UiCardEquipShowCardName(self, 0);
        UiCardEquipShowCardHelp(self, 0);
        self->base.phaseStep = 2;
      } else if (UiCardEquipCheckRowChange(self) == 1) { /* down to the gauge row */
        if (SndHasManager()) {
          SndManagerPlay(SndGetManager(), 1, 0, 0);
        }
        UiCardEquipSetRowFocus(self, (u8)self->row, 0);
        self->row = 2;
        UiCardEquipSetRowFocus(self, (u8)self->row, 1);
        UiCardEquipFollowCardMarkers(self);
        self->base.phaseStep = 7;
      } else if (UiCardEquipMoveCursor(self) == 1) {
        if (SndHasManager()) {
          SndManagerPlay(SndGetManager(), 1, 0, 0);
        }
        UiCardEquipSetRowFocus(self, (u8)self->row, 1);
        UiCardEquipFollowCardMarkers(self);
        UiCardEquipShowCardName(self, 1);
        UiCardEquipShowCardHelp(self, 1);
      } else {
        UiCardEquipFollowCardMarkers(self);
      }
    } else {
      if (result == 1) {
        if (SndHasManager()) {
          SndManagerPlay(SndGetManager(), 0, 0, 0);
        }
        UiCardEquipSetRowFocus(self, (u8)self->row, 1);
        UiCardEquipStartPress(self);
        self->base.phaseStep = 5;
      } else {
        if (SndHasManager()) {
          SndManagerPlay(SndGetManager(), 3, 0, 0);
        }
      }
      UiCardEquipFollowCardMarkers(self);
    }
    break;
  case 7: /* G-power gauge row */
    UiCardEquipUpdateGaugeRow(self);
    if ((self->base.pad->pressed & 0x2000) != 0) { /* Circle: back to the tab row */
      if (SndHasManager()) {
        SndManagerPlay(SndGetManager(), 2, 0, 0);
      }
      UiCardEquipSetRowFocus(self, 1, 0);
      self->row = 0;
      UiCardEquipSetRowFocus(self, (u8)self->row, 1);
      UiCardEquipFollowCardMarkers(self);
      UiCardEquipShowCardName(self, 0);
      UiCardEquipShowCardHelp(self, 0);
      self->base.phaseStep = 2;
    } else {
      if (UiCardEquipAdjustGauge(self) == 1) {
        if (SndHasManager()) {
          SndManagerPlay(SndGetManager(), 1, 0, 0);
        }
        UiCardEquipRefreshGaugeDigits(self);
        UiCardEquipInitGaugeBars(self);
        UiCardEquipStartPress(self);
        self->base.phaseStep = 5;
      }
      /* also checked after a gauge change, overriding step 5 */
      if (UiCardEquipCheckRowChange(self) == 1) {
        if (SndHasManager()) {
          SndManagerPlay(SndGetManager(), 1, 0, 0);
        }
        UiCardEquipSetRowFocus(self, (u8)self->row, 0);
        self->row = 1;
        UiCardEquipSetRowFocus(self, (u8)self->row, 1);
        self->base.phaseStep = 6;
      }
    }
    break;
  default:
    UiCardEquipSaveLoadout(self);
    self->base.phase = 4;
    self->base.phaseStep = 0;
    break;
  }
  UiCardEquipUpdatePointerArrows(self);
}
