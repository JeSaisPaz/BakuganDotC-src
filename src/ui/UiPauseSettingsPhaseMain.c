// bdc 0x089af1b8 UiPauseSettingsPhaseMain
#include "bdc.h"

/* Main phase (2) of `UiPauseSettings`, one `phaseStep` per frame:
   0 waits for the shared animation in slot 1 to end, then plays `main_light.fab` and starts every
   opening tween; 1 waits until the ten opening steppers all report done, then starts the arrows;
   2 waits for the arrows. 3 runs the menu: decide on the button row (sound 0, button flash, step
   5), cancel (sound 2, step 14), up/down and button-row left/right (sound 1), slider/toggle
   left/right (preview sound, arrow flash, step 4). 4 animates the pressed arrow. 5 acts on the
   decided button: 4 closes, 5 resets the defaults, 6 starts the return-to-title flow (step 10:
   fade, keep the BGM track in `savedBgmTrack`, stop the BGM; 11: fade voices, store the event
   flags, create task 10020 with profile flag 0x40000000 set; 12: wait for it; 13: replay the BGM).
   14 asks for confirmation when something changed; 15 reverts on "no". 6..9 run the closing
   tweens with `main_finish.fab` and fade to black; any other step ends the phase. */

void UiPauseSettingsPhaseMain(UiPauseSettings *self)
{
  GfxSprite **sprites;
  GfxFader *fader;
  GfxSprite *sprite;
  u32 frame;
  u8 done;
  u8 value;

  switch (self->base.phaseStep) {
  case 0:
    frame = UiSharedAnimGetFrame(self, 1);
    if (frame == UiSharedAnimGetLength(self, 1)) {
      UiSharedAnimRelease(self, 1);
      UiSharedAnimStart(900.0f, 0.0f, 0.0f, self, (void *)"main_light.fab", 1, 1);
      GfxFabUpdate(g_uiSharedAnims[1]);
      GfxFabUpdate(g_uiSharedAnims[1]);
      sprites = (GfxSprite **)self->base.data;
      UiTitlePlateInit(0, sprites[0x23]);
      UiPauseSettingsStartButtonTweens(self, 0);
      UiPauseSettingsStartTitleTween(self, 0);
      UiPauseSettingsStartLabelTweens(self, 0);
      UiPauseSettingsStartArrowSlides(self, 0);
      UiPauseSettingsStartToggleSlides(self, 0);
      UiPauseSettingsStartBarSlides(self, 0);
      UiPauseSettingsStartBgFade(self, 0);
      UiPauseSettingsStartGuideTweens(self, 0);
      UiPauseSettingsBeginToggle(self, 0);
      self->base.phaseStep++;
    }
    break;
  case 1:
    done = UiTitlePlateStep(0);
    done += UiPauseSettingsStepButtonTweens(self, 0);
    done += UiPauseSettingsStepTitleTween(self, 0);
    done += UiPauseSettingsStepLabelTweens(self, 0);
    done += UiPauseSettingsStepArrowSlides(self, 0);
    done += UiPauseSettingsStepToggleSlides(self, 0);
    done += UiPauseSettingsStepBarSlides(self, 0);
    done += UiPauseSettingsStepBgFade(self, 0);
    done += UiPauseSettingsStepHelpIcons(self, 0);
    done += UiPauseSettingsStepToggle(self, 0);
    if (done == 10) {
      UiPauseSettingsRefreshAllArrows(self);
      UiPauseSettingsBeginArrows(self, 0);
      self->base.phaseStep++;
    }
    break;
  case 2:
    if ((u8)UiPauseSettingsStepArrows(self, 0) == 1) {
      UiPauseSettingsRefreshCursor(self);
      self->base.phaseStep++;
    }
    break;
  case 3:
    UiPauseSettingsGlowCursor(self);
    UiPauseSettingsScaleSelectedButton(self);
    UiPauseSettingsPulseButtonHighlight(self);
    UiPulseStep(((GfxSprite **)self->base.data)[0x3f], &self->buttonPulse);
    if (UiPauseSettingsButtonDecided(self) == 1) {
      if (SndHasManager()) {
        SndManagerPlay(SndGetManager(), 0, 0, 0);
      }
      UiPauseSettingsRefreshCursor(self);
      UiPauseSettingsStartButtonFlash(self);
      self->base.phaseStep = 5;
    } else if (UiPauseSettingsCancelPressed(self) == 1) {
      if (SndHasManager()) {
        SndManagerPlay(SndGetManager(), 2, 0, 0);
      }
      self->base.phaseStep = 14;
    } else if (UiPauseSettingsHandleUpDown(self) == 1) {
      if (SndHasManager()) {
        SndManagerPlay(SndGetManager(), 1, 0, 0);
      }
      UiPauseSettingsRefreshCursor(self);
    } else if (UiPauseSettingsHandleLeftRight(self) == 1) {
      UiPauseSettingsPlayPreview(self);
      UiPauseSettingsFlashArrow(self);
      UiPauseSettingsRefreshRowValue(self);
      self->base.phaseStep = 4;
    } else if (UiPauseSettingsHandleButtonsLeftRight(self) == 1) {
      if (SndHasManager()) {
        SndManagerPlay(SndGetManager(), 1, 0, 0);
      }
      UiPauseSettingsRefreshCursor(self);
    }
    break;
  case 4:
    UiPauseSettingsGlowCursor(self);
    if (UiPauseSettingsStepArrowPress(self) == 1) {
      UiPauseSettingsRefreshRowArrows(self, (u8)self->cursor);
      self->base.phaseStep = 3;
    }
    break;
  case 5:
    if (UiPauseSettingsButtonFlashDone(self) == 1) {
      switch (self->cursor) {
      case 4: /* close */
        UiPauseSettingsHideHighlight(self);
        self->base.phaseStep = 6;
        break;
      case 5: /* reset to defaults */
        UiPauseSettingsResetDefaults(self);
        sprite = ((GfxSprite **)self->base.data)[0x24];
        value = UiPauseSettingsGetItemValue(self, 0);
        UiPauseSettingsSetBarWidth(self, sprite, value);
        sprite = ((GfxSprite **)self->base.data)[0x25];
        value = UiPauseSettingsGetItemValue(self, 1);
        UiPauseSettingsSetBarWidth(self, sprite, value);
        sprite = ((GfxSprite **)self->base.data)[0x26];
        value = UiPauseSettingsGetItemValue(self, 2);
        UiPauseSettingsSetBarWidth(self, sprite, value);
        sprite = ((GfxSprite **)self->base.data)[0x3d];
        UiPauseSettingsSetToggleCell(self, sprite, SaveGetProfile()->data->adviceOff);
        UiPauseSettingsRefreshAllArrows(self);
        self->base.phaseStep = 3;
        break;
      case 6: /* return to title */
        self->base.phaseStep = 10;
        break;
      default:
        break;
      }
    }
    break;
  case 6:
    UiPauseSettingsStartBgFade(self, 1);
    UiTitlePlateInit(1, ((GfxSprite **)self->base.data)[0x23]);
    UiPauseSettingsStartButtonTweens(self, 1);
    UiPauseSettingsStartTitleTween(self, 1);
    UiPauseSettingsStartLabelTweens(self, 1);
    UiPauseSettingsStartArrowSlides(self, 1);
    UiPauseSettingsStartToggleSlides(self, 1);
    UiPauseSettingsStartBarSlides(self, 1);
    UiPauseSettingsBeginArrows(self, 1);
    UiPauseSettingsStartGuideTweens(self, 1);
    UiPauseSettingsBeginToggle(self, 1);
    UiSharedAnimRelease(self, 1);
    UiSharedAnimStart(900.0f, 0.0f, 0.0f, self, (void *)"main_finish.fab", 1, 0);
    GfxFabUpdate(g_uiSharedAnims[1]);
    GfxFabUpdate(g_uiSharedAnims[1]);
    self->base.phaseStep++;
    break;
  case 7:
    done = UiPauseSettingsStepBgFade(self, 1);
    done += UiTitlePlateStep(1);
    done += UiPauseSettingsStepButtonTweens(self, 1);
    done += UiPauseSettingsStepTitleTween(self, 1);
    done += UiPauseSettingsStepLabelTweens(self, 1);
    done += UiPauseSettingsStepArrowSlides(self, 1);
    done += UiPauseSettingsStepToggleSlides(self, 1);
    done += UiPauseSettingsStepBarSlides(self, 1);
    done += UiPauseSettingsStepArrows(self, 1);
    done += UiPauseSettingsStepHelpIcons(self, 1);
    done += UiPauseSettingsStepToggle(self, 1);
    if (done == 11) {
      self->base.phaseStep = 8;
    }
    break;
  case 8:
    frame = UiSharedAnimGetFrame(self, 1);
    if (frame == UiSharedAnimGetLength(self, 1)) {
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
    }
    break;
  case 9:
    if (GfxFaderIsFinished(GfxGetActiveFader())) {
      self->base.phaseStep = 16;
    }
    break;
  case 10:
    fader = GfxGetActiveFader();
    fader->start[0] = 0.0f;
    fader->start[1] = 0.0f;
    fader->start[2] = 0.0f;
    fader->start[3] = 0.0f;
    fader = GfxGetActiveFader();
    fader->end[0] = 0.26667f;
    fader->end[1] = 0.53333f;
    fader->end[2] = 0.26667f;
    fader->end[3] = 0.8f;
    GfxFaderSetPreset(GfxGetActiveFader(), 3);
    GfxFaderStart(GfxGetActiveFader(), 15);
    self->savedBgmTrack = SndBgmPlayerGetTrackId(SndBgmPlayerGet(0));
    SndBgmCancelChannel(0);
    SndBgmQueueStop(0.1f, 0);
    self->base.phaseStep++;
    break;
  case 11:
    if (GfxFaderIsFinished(GfxGetActiveFader())) {
      SndManagerFadeOutAllVoices(SndGetManager());
      if (g_saveSnapshotEnabled != 0) {
        SaveProfileStoreEventFlags();
      }
      UiMenuFlagsModify(0, 2);
      SaveProfileSetFlags(SaveGetProfile(), 0x40000000);
      CoreTaskCreate(10020, 100);
      self->base.phaseStep = 12;
    }
    break;
  case 12:
    if (CoreTaskExists(10020) == 0) {
      SaveProfileClearFlags(SaveGetProfile(), 0x40000000);
      fader = GfxGetActiveFader();
      fader->end[0] = 0.0f;
      fader->end[1] = 0.0f;
      fader->end[2] = 0.0f;
      fader->end[3] = 0.0f;
      self->base.phaseStep = 13;
    }
    break;
  case 13:
    SndBgmQueuePlay(0, self->savedBgmTrack, 1, 0);
    UiPauseSettingsRefreshCursor(self);
    if (UiMenuFlagsTest(2) != 0) {
      UiPauseSettingsSnapshotValues(self);
    }
    self->base.phaseStep = 3;
    break;
  case 14:
    if (UiPauseSettingsHasChanges(self) == 1) {
      UiPauseSettingsConfirmBegin(self, 8);
      self->base.phaseStep = 15;
    } else {
      UiPauseSettingsHideHighlight(self);
      self->base.phaseStep = 6;
    }
    break;
  case 15:
    if (UiPauseSettingsConfirmStep(self) == 1) {
      if (self->dlgResult == 0) {
        UiPauseSettingsRevertChanges(self);
        UiPauseSettingsHideHighlight(self);
        self->base.phaseStep = 6;
      } else {
        self->base.phaseStep = 3;
      }
    }
    break;
  default:
    UiPauseSettingsSetResultNone(self);
    UiSharedBgClose();
    g_uiKeepSharedBg = 0;
    self->base.phaseStep = 0;
    self->base.phase++;
    break;
  }
}
