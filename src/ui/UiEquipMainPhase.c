// bdc 0x08966598 UiEquipMainPhase
#include "bdc.h"

/* Offline main phase (entry 2 of the phase table `0x08a9d5f8`) of the Bakugan/gear loadout screen
   before a battle (task 302, `UiEquipCtor`; per player picks Bakugan from a 20-face
   grid (`"baku_face_%02d"`) and equipment (`"c_setting_soubi_l_sol_%02d"`), shown on
   `"menu_daiza.gmo"` pedestals; `+0x4cda` = number of players (2 or 4 layouts), `+0x4cdb` = player
   being edited): opens all parts, then for each player lets them pick a Bakugan on the grid (move,
   confirm, random pick `UiEquipRandomPick`, help `UiEquipShowHelpPopup`, cancel back to the
   previous player) and its equipment in the equipment panel (`UiEquipStartGearPanelTween` …);
   when all players are done closes everything (`"main_finish.fab"`), fades out and switches to
   phase 4 after `UiEquipSaveSelections` and `UiSharedBgClose`. */

/* plays UI sound `id` when the sound manager exists (inlined at every use in the original) */
#define UiEquipMainPhasePlaySe(id)                      \
  do {                                                  \
    if (SndHasManager()) {                              \
      SndManagerPlay(SndGetManager(), (id), 0, 0);      \
    }                                                   \
  } while (0)

void UiEquipMainPhase(UiEquip *self)
{
  GfxFader *fader;
  u8 done;
  u8 result;

  UiEquipUpdateAnimations(self);
  switch (self->base.phaseStep) {
  case 0:
    if (UiSharedAnimGetFrame(self, 1) == UiSharedAnimGetLength(self, 1)) {
      UiSharedAnimRelease(self, 1);
      UiSharedAnimStart(300.0f, 0.0f, 0.0f, self, (void *)"main_light.fab", 1, 1);
      GfxFabUpdate(g_uiSharedAnims[1]);
      GfxFabUpdate(g_uiSharedAnims[1]);
      UiTitlePlateInit(0, ((GfxSprite **)self->base.data)[0]);
      UiEquipStartEmblemZoomIn(self);
      UiEquipSetFrameFlags(self, true, 1);
      self->base.phaseStep++;
    }
    break;
  case 1:
    done = UiTitlePlateStep(0);
    done += UiEquipUpdateEmblemZoomIn(self);
    if (done == 2) {
      UiEquipStartEmblemHold(self);
      self->base.phaseStep++;
    }
    break;
  case 2:
    if (UiEquipUpdateEmblemHold(self)) {
      UiEquipStartPlayerIconsTween(self, 0);
      UiEquipPrepareEmblemTween(self, false);
      UiEquipStartPlayerLabelsTween(self, 0);
      UiEquipStartPlayerDoneMarksTween(self, 0);
      UiEquipStartButtonGuideTween(self, 0);
      UiEquipResetIntroCounter(self);
      self->base.phaseStep++;
    }
    break;
  case 3:
    UiEquipStepIntroCounter(self);
    done = (u8)UiEquipUpdateEmblemTween(self, false);
    done += UiEquipUpdatePlayerIconsTween(self, 0);
    done += UiEquipUpdatePlayerLabelsTween(self, 0);
    done += UiEquipUpdatePlayerDoneMarksTween(self, 0);
    done += UiEquipUpdateButtonGuideTween(self, 0);
    if (done == 5) {
      UiEquipStartGridPanelTween(self, 0);
      UiEquipStartRandomButtonTween(self, 0);
      UiEquipStartSprite516aTween(self, 0);
      UiEquipStartGridTween(self, 0);
      UiEquipStartFourPlayerDecorTween(self, 0);
      UiEquipStartPlayerTabTween(self, 0);
      UiEquipStartDoneSlotsTween(self, 0);
      self->base.phaseStep++;
    }
    break;
  case 4:
    done = UiEquipUpdateGridPanelTween(self, 0);
    done += UiEquipUpdateRandomButtonTween(self, 0);
    done += UiEquipUpdateSprite516aTween(self, 0);
    done += UiEquipUpdateGridTween(self, 0);
    done += UiEquipUpdateFourPlayerDecorTween(self, 0);
    done += UiEquipUpdatePlayerTabTween(self, 0);
    done += UiEquipUpdateDoneSlotsTween(self, 0);
    if (done == 7) {
      UiEquipSetPendingPulse(self, true);
      UiEquipStartAllPlayerStamps(self, false);
      UiEquipSetFrameFlags(self, true, 4);
      if (self->reEditLast != 0) {
        /* entered from task 310: reopen the last player's equipment panel */
        UiEquipResetCursor(self);
        self->doneCount--;
        self->reEditLast = 0;
        self->gearEditing = 0;
        UiEquipHidePlayerRows(self, self->editPlayer);
        UiEquipHideGridCursors(self);
        self->base.phaseStep = 0xe;
      } else {
        UiEquipResetCursor(self);
        self->base.phaseStep++;
      }
    }
    break;
  case 5:
    UiEquipUpdateCursorPulse(self);
    UiEquipPulseGridCursor(self);
    UiPulseStep(((GfxSprite **)self->base.data)[self->spriteCount], (UiPulse *)&self->tweens[self->spriteCount]);
    result = (u8)UiEquipCheckGridConfirm(self);
    if (result == 0) {
      if ((self->base.pad->pressed & 0x2000) != 0) {
        /* cancel */
        UiEquipMainPhasePlaySe(2);
        UiEquipResetCursor(self);
        if (self->editPlayer == 0) {
          self->gearEditing = 1;
          UiEquipHideGridCursors(self);
          UiEquipSetupSlotModels(self, true);
          UiEquipStartPlayerStamp(self, self->editPlayer, 0);
          self->base.phaseStep = 7;
        } else {
          UiEquipShowPlayerSlot(self, false, self->editPlayer);
          UiEquipStepBackPlayer(self);
          UiEquipResetCursor(self);
          UiEquipHidePlayerRows(self, self->editPlayer);
          UiEquipShowPlayerSlot(self, true, self->editPlayer);
        }
      } else if (UiEquipMoveGridCursor(self) == 1) {
        UiEquipMainPhasePlaySe(1);
        UiEquipResetCursor(self);
        UiEquipFreeCurrentModel(self);
        self->base.phaseStep = 6;
      } else if (UiEquipAdjustHandicap(self, self->editPlayer) == 1) {
        UiEquipMainPhasePlaySe(1);
        UiEquipRefreshHandicapStars(self, self->editPlayer);
      }
    } else if (result == 1) {
      UiEquipMainPhasePlaySe(0);
      UiEquipResetCursor(self);
      UiEquipFlashGridSelection(self);
      self->gearEditing = 0;
      self->base.phaseStep = 0xb;
    } else {
      UiEquipMainPhasePlaySe(3);
    }
    break;
  case 6:
    if (self->modelSwapDelay != 0) {
      self->modelSwapDelay--;
    } else {
      UiEquipPreviewHoveredBakugan(self);
      self->base.phaseStep = 5;
    }
    break;
  case 7:
    UiEquipHideSelectionMarkers(self);
    UiEquipSetPendingPulse(self, false);
    UiEquipStartAllPlayerStamps(self, true);
    UiTitlePlateInit(1, ((GfxSprite **)self->base.data)[0]);
    UiEquipPrepareEmblemTween(self, true);
    UiEquipStartPlayerIconsTween(self, 1);
    UiEquipStartPlayerLabelsTween(self, 1);
    UiEquipStartGridPanelTween(self, 1);
    UiEquipStartRandomButtonTween(self, 1);
    UiEquipStartSprite516aTween(self, 1);
    UiEquipStartGridTween(self, 1);
    UiEquipStartPlayerDoneMarksTween(self, 1);
    UiEquipStartFourPlayerDecorTween(self, 1);
    UiEquipStartPlayerTabTween(self, 1);
    UiEquipStartDoneSlotsTween(self, 1);
    UiEquipStartButtonGuideTween(self, 1);
    UiSharedAnimRelease(self, 1);
    UiSharedAnimStart(300.0f, 0.0f, 0.0f, self, (void *)"main_finish.fab", 1, 0);
    GfxFabUpdate(g_uiSharedAnims[1]);
    GfxFabUpdate(g_uiSharedAnims[1]);
    self->base.phaseStep++;
    break;
  case 8:
    done = UiTitlePlateStep(1);
    done += (u8)UiEquipUpdateEmblemTween(self, true);
    done += UiEquipUpdatePlayerIconsTween(self, 1);
    done += UiEquipUpdatePlayerLabelsTween(self, 1);
    done += UiEquipUpdateGridPanelTween(self, 1);
    done += UiEquipUpdateRandomButtonTween(self, 1);
    done += UiEquipUpdateSprite516aTween(self, 1);
    done += UiEquipUpdateGridTween(self, 1);
    done += UiEquipUpdatePlayerDoneMarksTween(self, 1);
    done += UiEquipUpdateFourPlayerDecorTween(self, 1);
    done += UiEquipUpdatePlayerTabTween(self, 1);
    done += UiEquipUpdateDoneSlotsTween(self, 1);
    done += UiEquipUpdateButtonGuideTween(self, 1);
    if (done == 0xd) {
      self->base.phaseStep++;
    }
    break;
  case 9:
    if (!((s32)UiSharedAnimGetFrame(self, 1) < (s32)UiSharedAnimGetLength(self, 1))) {
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
      GfxFaderStart(GfxGetActiveFader(), 8);
      self->base.phaseStep++;
    }
    break;
  case 10:
    if (GfxFaderIsFinished(GfxGetActiveFader())) {
      self->base.phaseStep = 0x17;
    }
    break;
  case 0xb:
    if (UiEquipWaitGridFlash(self) == 1) {
      if (self->onRandom == 0) {
        UiEquipHideGridCursors(self);
        self->base.phaseStep = 0xe;
      } else {
        UiEquipShowPlayerGroup51e2(self, self->editPlayer, false);
        UiEquipResetRandomPick(self);
        self->base.phaseStep = 0xc;
      }
    }
    break;
  case 0xc:
    if (UiEquipRandomPick(self) == 1) {
      UiEquipMainPhasePlaySe(0);
      UiEquipResetCursor(self);
      UiEquipFlashGridSelection(self);
      self->gearEditing = 0;
      self->base.phaseStep = 0xb;
    }
    break;
  case 0xd:
    if (UiEquipWaitGridFlash(self) == 1) {
      UiEquipHideGridCursors(self);
      self->base.phaseStep = 0x14;
    }
    break;
  case 0xe:
    UiEquipResetGearPanelCursor(self);
    UiEquipShowPlayerGroup51e2(self, self->editPlayer, false);
    UiEquipStartGearPanelTween(self, false, self->editPlayer);
    self->base.phaseStep++;
    break;
  case 0xf:
    if (UiEquipUpdateGearPanelTween(self, false, self->editPlayer)) {
      UiEquipResetGearCursor(self, self->editPlayer);
      self->base.phaseStep = 0x10;
    }
    break;
  case 0x10:
    UiEquipPulseGearCursor(self, self->editPlayer);
    UiEquipGlowOkButton(self, self->editPlayer);
    UiEquipUpdateGearCursorPulse(self, self->editPlayer);
    UiPulseStep(((GfxSprite **)self->base.data)[self->spriteCount], (UiPulse *)&self->tweens[self->spriteCount]);
    result = (u8)UiEquipCheckGearConfirm(self);
    if (result == 0) {
      if ((self->base.pad->pressed & 0x2000) != 0) {
        /* cancel: close the panel and go back to the grid */
        UiEquipMainPhasePlaySe(2);
        self->gearReturnToGrid = 1;
        UiEquipHideGearCursors(self, self->editPlayer);
        self->base.phaseStep = 0x11;
      } else if (UiEquipMoveGearRow(self) == 1) {
        UiEquipMainPhasePlaySe(1);
        UiEquipResetGearCursor(self, self->editPlayer);
      } else if (UiEquipMoveGearCursor(self) == 1) {
        UiEquipMainPhasePlaySe(1);
        UiEquipResetGearCursor(self, self->editPlayer);
      } else {
        result = (u8)UiEquipCheckHelpButton(self);
        if (result != 0) {
          if (result == 1) {
            UiEquipMainPhasePlaySe(0);
            UiEquipResetHelpPopup(self);
            self->base.phaseStep = 0x16;
          } else {
            UiEquipMainPhasePlaySe(3);
          }
        }
      }
    } else if (result == 1) {
      UiEquipMainPhasePlaySe(0);
      UiEquipResetGearCursor(self, self->editPlayer);
      UiEquipFlashGearSelection(self);
      self->gearReturnToGrid = 0;
      self->base.phaseStep = 0x13;
    } else {
      UiEquipMainPhasePlaySe(3);
    }
    break;
  case 0x11:
    UiEquipStartGearPanelTween(self, true, self->editPlayer);
    self->base.phaseStep++;
    break;
  case 0x12:
    if (UiEquipUpdateGearPanelTween(self, true, self->editPlayer)) {
      if (self->gearReturnToGrid != 0) {
        UiEquipResetCursor(self);
        UiEquipShowPlayerGroup51e2(self, self->editPlayer, true);
        self->base.phaseStep = 5;
      } else {
        self->base.phaseStep = 0x14;
      }
    }
    break;
  case 0x13:
    if (UiEquipWaitGearFlash(self) == 1) {
      if (self->activeRow == 0) {
        UiEquipRefreshGearList(self, self->editPlayer);
        self->base.phaseStep = 0x10;
      } else {
        UiEquipHideGearCursors(self, self->editPlayer);
        self->base.phaseStep = 0x11;
      }
    }
    break;
  case 0x14:
    UiEquipStartCommitTween(self, self->editPlayer);
    self->base.phaseStep++;
    break;
  case 0x15:
    if (UiEquipUpdateCommitTween(self, self->editPlayer)) {
      if (UiEquipCommitPlayerBakugan(self) == 1) {
        UiEquipSetupSlotModels(self, true);
        self->base.phaseStep = 7;
      } else {
        UiEquipResetCursor(self);
        UiEquipFreeCurrentModel(self);
        UiEquipShowPlayerSlot(self, true, self->editPlayer);
        self->base.phaseStep = 6;
      }
    }
    break;
  case 0x16:
    if (UiEquipShowHelpPopup(self) == 1) {
      self->base.phaseStep = 0x10;
    }
    break;
  default:
    UiEquipSaveSelections(self);
    UiSharedBgClose();
    g_uiKeepSharedBg = 0;
    self->base.phase = 4;
    self->base.phaseStep = 0;
    break;
  }
  UiEquipLayoutGrid(self);
  UiEquipLayoutPlayerMarkers(self);
  UiEquipPulsePendingPlayers(self);
  UiEquipAnimateUnknownMarkers(self);
}
