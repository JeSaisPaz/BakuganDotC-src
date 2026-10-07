// bdc 0x089aaf48 UiMainMenuPhaseMain
#include "bdc.h"

/* Phase 2 of `UiMainMenu` (the carousel): plays `main_light.fab`, runs the opening
   animations (title plate, item/arrow slides, frame and model fades, button tweens), then handles
   input — left/right rotate the carousel (`UiMainMenuHandleCursorInput`,
   `UiMainMenuStartCarouselMove`), Cross on an unlocked item flashes it and leaves (sound 0),
   Circle (pad `buttons & 0x2000`) cancels (sound 2) — shows pending first-time help
   (`UiMainMenuRunHelp`), plays `main_finish.fab` and the closing animations, fades out, then
   applies the selection (`UiMainMenuApplySelection`) and moves to phase 6 (or phase 3 for
   item 1, the battle entry). */

void UiMainMenuPhaseMain(UiMainMenu *self)
{
  u8 sum;
  int decision;
  s32 next;
  s8 cursor;
  GfxFader *fader;

  UiMainMenuBobModels(self);
  switch (self->base.phaseStep) {
  case 0:
    if ((int)UiSharedAnimGetFrame(self, 1) >= (int)UiSharedAnimGetLength(self, 1)) {
      UiSharedAnimRelease(self, 1);
      UiSharedAnimStart(100.0f, 0.0f, 0.0f, self, (void *)"main_light.fab", 1, 1);
      GfxFabUpdate(g_uiSharedAnims[1]);
      GfxFabUpdate(g_uiSharedAnims[1]);
      UiTitlePlateInit(0, ((GfxSprite **)self->base.data)[2]);
      self->base.phaseStep = self->base.phaseStep + 1;
    }
    UiMainMenuStepItemSlide(self, 0);
    break;
  case 1:
    sum = UiTitlePlateStep(0);
    sum = (u8)(sum + UiMainMenuStepItemSlide(self, 0));
    if (sum == 2) {
      UiMainMenuStartArrowSlide(self, 0);
      UiMainMenuStartFrameFade(self, 0);
      UiMainMenuStartModelFade(self, 0);
      UiMainMenuStartButtonTweens(self, 0);
      self->base.phaseStep = self->base.phaseStep + 1;
    }
    break;
  case 2:
    sum = (u8)UiMainMenuStepArrowSlide(self, 0);
    sum = (u8)(sum + UiMainMenuStepFrameFade(self, 0));
    sum = (u8)(sum + UiMainMenuStepModelFade(self, 0));
    sum = (u8)(sum + UiMainMenuStepButtonTweens(self, 0));
    if (sum == 4) {
      UiMainMenuShowItemLabel(self, 1, (u8)self->cursor);
      self->bobbing = 1;
      self->base.phaseStep = self->base.phaseStep + 1;
      if ((SaveGetProfile()->data->playthroughClearBits & 1) != 0) {
        UiMainMenuStartHelp(self, 4);
        self->base.phaseStep = 12;
      } else if ((SaveGetProfile()->data->playthroughClearBits & 2) != 0) {
        UiMainMenuStartHelp(self, 7);
        self->base.phaseStep = 12;
      }
    }
    break;
  case 3:
    UiMainMenuPulseItemLabel(self, (u8)self->cursor);
    decision = (u8)UiMainMenuGetDecision(self);
    if (decision == 0) {
      if ((self->base.pad->buttons & 0x2000) != 0) {
        if (SndHasManager()) {
          SndManagerPlay(SndGetManager(), 2, 0, 0);
        }
        self->cancelled = 1;
        UiMainMenuStartHelp(self, 3);
        self->base.phaseStep = 12;
        return;
      }
      if (UiMainMenuHandleCursorInput(self) == 1) {
        if (SndHasManager()) {
          SndManagerPlay(SndGetManager(), 1, 0, 0);
        }
        UiMainMenuStartCarouselMove(self);
        self->base.phaseStep = 4;
        return;
      }
    } else if (decision == 1) {
      if (SndHasManager()) {
        SndManagerPlay(SndGetManager(), 0, 0, 0);
      }
      UiMainMenuStartDecideFlash(self);
      self->cancelled = 0;
      self->base.phaseStep = 5;
      return;
    } else {
      /* locked item */
      if (SndHasManager()) {
        SndManagerPlay(SndGetManager(), 3, 0, 0);
      }
    }
    break;
  case 4:
    if (UiMainMenuStepCarousel(self) == 1) {
      UiMainMenuShowItemLabel(self, 1, (u8)self->cursor);
      self->base.phaseStep = 3;
    }
    break;
  case 5:
    if (UiMainMenuDecideFlashDone(self) != 0) {
      next = 6;
      if (self->cursor == 1) {
        next = 10;
      }
      self->base.phaseStep = next;
    }
    break;
  case 6:
    UiMainMenuInitItemSprites(self, 1);
    UiTitlePlateInit(1, ((GfxSprite **)self->base.data)[2]);
    UiMainMenuStartArrowSlide(self, 1);
    UiMainMenuStartFrameFade(self, 1);
    UiMainMenuStartModelFade(self, 1);
    UiMainMenuStartButtonTweens(self, 1);
    UiSharedAnimRelease(self, 1);
    UiSharedAnimStart(100.0f, 0.0f, 0.0f, self, (void *)"main_finish.fab", 1, 0);
    GfxFabUpdate(g_uiSharedAnims[1]);
    GfxFabUpdate(g_uiSharedAnims[1]);
    self->base.phaseStep = self->base.phaseStep + 1;
    break;
  case 7:
    sum = UiTitlePlateStep(1);
    sum = (u8)(sum + UiMainMenuStepItemSlide(self, 1));
    sum = (u8)(sum + UiMainMenuStepArrowSlide(self, 1));
    sum = (u8)(sum + UiMainMenuStepFrameFade(self, 1));
    sum = (u8)(sum + UiMainMenuStepModelFade(self, 1));
    sum = (u8)(sum + UiMainMenuStepButtonTweens(self, 1));
    if (sum == 6) {
      self->base.phaseStep = 8;
    }
    break;
  case 8:
    if ((int)UiSharedAnimGetFrame(self, 1) >= (int)UiSharedAnimGetLength(self, 1)) {
      if (self->cancelled != 0) {
        SndBgmCancelChannel(0);
        SndBgmQueueStop(0.1f, 0);
      } else {
        cursor = self->cursor;
        if (cursor == 0 || cursor == 4 || cursor == 3) {
          SndBgmCancelChannel(0);
          SndBgmQueueStop(0.1f, 0);
        }
      }
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
      self->base.phaseStep = self->base.phaseStep + 1;
    }
    break;
  case 9:
    if (GfxFaderIsFinished(GfxGetActiveFader())) {
      self->base.phaseStep = 13;
    }
    break;
  case 10:
    self->base.phase = 3;
    self->base.phaseStep = 0;
    break;
  case 11:
    if (UiCommonNoticeRun() == 1) {
      self->base.phaseStep = 3;
    }
    break;
  case 12:
    if (UiMainMenuRunHelp(self) == 1) {
      next = 3;
      if (self->helpResult == 0) {
        next = 6;
      }
      self->base.phaseStep = next;
    }
    break;
  default:
    UiMainMenuApplySelection(self);
    if (UiMainMenuShouldCloseSharedBg(self) != 0) {
      UiSharedBgClose();
      g_uiKeepSharedBg = 0;
    } else {
      g_uiKeepSharedBg = 1;
    }
    self->base.phase = 6;
    self->base.phaseStep = 0;
    break;
  }
}
