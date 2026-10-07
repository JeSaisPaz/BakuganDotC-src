// bdc 0x08977e54 UiCollectionMenuMainPhase
#include "bdc.h"

/* Main phase (entry 2 of the phase table `0x08a9db18`) of the collection top menu (task 311,
   `maybe_UiScreen311Ctor`; entries `"Card_light"`, `"Figure_light"`, `"Sphere_light"`,
   `"Theater_light"`, `"Mark_light"`, `"Reset_light"`, a rotating `"menu_itembox.gmo"` model; page
   `+0x503` 0 = main entries, 1 = sub-page; selection `screen+0x500[page]`, enabled-entry mask
   `+0x50c`): opens buttons, item box and title (`"main_light.fab"`), handles cursor movement
   (`UiCollectionMenuMoveCursor`), confirm (`UiCollectionMenuCheckConfirm` →
   `UiCollectionMenuStartPress`, then `UiCollectionMenuConfirmEntry` picks the follow-up
   sub-state per entry: open a collection screen or the sub-page) and cancel (closes with
   `"main_finish.fab"`, BGM stop, fade); sub-states 8..0x11 handle the sub-page and returning from a
   collection screen. Every frame ends with `UiCollectionMenuUpdateLightBlink`. */

void UiCollectionMenuMainPhase(UiCollectionMenu *self)
{
  GfxSprite **sprites;
  GfxFader *fader;
  u32 frame;
  u8 done;
  u8 confirm;
  s8 sel;

  switch (self->base.phaseStep) {
  case 0:
    frame = UiSharedAnimGetFrame(self, 1);
    if (frame == UiSharedAnimGetLength(self, 1)) {
      UiSharedAnimRelease(self, 1);
      UiSharedAnimStart(15.0f, 0.0f, 0.0f, self, "main_light.fab", 1, 1);
      GfxFabUpdate(g_uiSharedAnims[1]);
      GfxFabUpdate(g_uiSharedAnims[1]);
      sprites = (GfxSprite **)self->base.data;
      self->slideDuration = 16.0f;
      UiTitlePlateInit(0, sprites[0]);
      UiCollectionMenuStartTitleSlide(self, 0);
      UiCollectionMenuStartButtonSlide(self, 0);
      UiCollectionMenuStartItemBoxTween(self, 0);
      UiCollectionMenuStartFrameTween(self, 0);
      UiCollectionMenuStartItemBoxMotion(self, 0);
      self->base.phaseStep++;
    }
    break;
  case 1:
    done = UiTitlePlateStep(0);
    done += UiCollectionMenuUpdateTitleSlide(self, 0);
    done += UiCollectionMenuUpdateButtonSlide(self, 0);
    done += UiCollectionMenuUpdateItemBoxTween(self, false);
    done += UiCollectionMenuUpdateFrameTween(self, 0);
    if (done == 5) {
      UiCollectionMenuStartLightBlink(self);
      UiCollectionMenuResetCursor(self);
      self->base.phaseStep++;
    }
    break;
  case 2:
    UiCollectionMenuPulseCursor(self);
    UiCollectionMenuAnimateButton(self);
    UiCollectionMenuUpdateCursorPulse(self);
    UiPulseStep(((GfxSprite **)self->base.data)[25], (UiPulse *)&self->tweens[25]);
    confirm = (u8)UiCollectionMenuCheckConfirm(self);
    if (confirm == 0) {
      if ((self->base.pad->pressed & 0x2000) != 0) {
        /* Circle: close the menu after a 16-frame delay. */
        if (SndHasManager()) {
          SndManagerPlay(SndGetManager(), 2, 0, 0);
        }
        self->cancelled = 1;
        UiCollectionMenuStartItemBoxMotion(self, 1);
        self->closeDelay = 0x10;
        self->base.phaseStep = 3;
      } else if (UiCollectionMenuMoveCursor(self) == 1) {
        if (SndHasManager()) {
          SndManagerPlay(SndGetManager(), 1, 0, 0);
        }
        UiCollectionMenuResetCursor(self);
      }
    } else if (confirm == 1) {
      if (SndHasManager()) {
        SndManagerPlay(SndGetManager(), 0, 0, 0);
      }
      UiCollectionMenuResetCursor(self);
      UiCollectionMenuStartPress(self);
      self->cancelled = 0;
      self->base.phaseStep = 7;
    } else {
      if (SndHasManager()) {
        SndManagerPlay(SndGetManager(), 3, 0, 0);
      }
    }
    break;
  case 3:
    if (self->closeDelay != 0) {
      self->closeDelay--;
      break;
    }
    UiCollectionMenuHideCursor(self);
    UiTitlePlateInit(1, ((GfxSprite **)self->base.data)[0]);
    UiCollectionMenuStartTitleSlide(self, 1);
    UiCollectionMenuStartButtonSlide(self, 1);
    UiCollectionMenuStartItemBoxTween(self, 1);
    UiCollectionMenuStartFrameTween(self, 1);
    UiSharedAnimRelease(self, 1);
    UiSharedAnimStart(15.0f, 0.0f, 0.0f, self, "main_finish.fab", 1, 0);
    GfxFabUpdate(g_uiSharedAnims[1]);
    GfxFabUpdate(g_uiSharedAnims[1]);
    self->base.phaseStep++;
    break;
  case 4:
    done = UiTitlePlateStep(1);
    done += UiCollectionMenuUpdateTitleSlide(self, 1);
    done += UiCollectionMenuUpdateButtonSlide(self, 1);
    done += UiCollectionMenuUpdateItemBoxTween(self, true);
    done += UiCollectionMenuUpdateFrameTween(self, 1);
    if (done == 5) {
      self->base.phaseStep = 5;
    }
    break;
  case 5:
    frame = UiSharedAnimGetFrame(self, 1);
    if ((s32)frame >= (s32)UiSharedAnimGetLength(self, 1)) {
      SndBgmCancelChannel(0);
      SndBgmQueueStop(0.1f, 0);
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
      GfxFaderStart(GfxGetActiveFader(), 0x10);
      self->base.phaseStep++;
    }
    break;
  case 6:
    if (GfxFaderIsFinished(GfxGetActiveFader())) {
      self->base.phaseStep = 0x12;
    }
    break;
  case 7:
    if (UiCollectionMenuWaitPress(self) == 1) {
      UiCollectionMenuConfirmEntry(self);
    }
    break;
  case 8:
    UiCollectionMenuStartButtonSlide(self, 0);
    self->base.phaseStep++;
    break;
  case 9:
    if ((u8)UiCollectionMenuUpdateButtonSlide(self, 0) == 1) {
      UiCollectionMenuResetCursor(self);
      self->base.phaseStep = (self->page == 0) ? 2 : 10;
    }
    break;
  case 10:
    UiCollectionMenuPulseCursor(self);
    UiCollectionMenuAnimateButton(self);
    UiCollectionMenuUpdateCursorPulse(self);
    UiPulseStep(((GfxSprite **)self->base.data)[25], (UiPulse *)&self->tweens[25]);
    confirm = (u8)UiCollectionMenuCheckConfirm(self);
    if (confirm == 0) {
      if ((self->base.pad->pressed & 0x2000) != 0) {
        /* Circle: back to the main page. */
        UiCollectionMenuRestoreItemBoxMotion(self, 0);
        if (SndHasManager()) {
          SndManagerPlay(SndGetManager(), 2, 0, 0);
        }
        UiCollectionMenuHideCursor(self);
        self->base.phaseStep = 0xb;
      } else if (UiCollectionMenuMoveCursor(self) == 1) {
        UiCollectionMenuPlaySubEntryMotion(self);
        if (SndHasManager()) {
          SndManagerPlay(SndGetManager(), 1, 0, 0);
        }
        UiCollectionMenuResetCursor(self);
      }
    } else if (confirm == 1) {
      if (SndHasManager()) {
        SndManagerPlay(SndGetManager(), 0, 0, 0);
      }
      UiCollectionMenuResetCursor(self);
      UiCollectionMenuStartPress(self);
      self->cancelled = 0;
      self->base.phaseStep = 7;
    } else {
      if (SndHasManager()) {
        SndManagerPlay(SndGetManager(), 3, 0, 0);
      }
    }
    break;
  case 0xb:
    UiCollectionMenuStartButtonSlide(self, 1);
    self->base.phaseStep++;
    break;
  case 0xc:
    if ((u8)UiCollectionMenuUpdateButtonSlide(self, 1) == 1) {
      self->page ^= 1;
      self->base.phaseStep = 8;
    }
    break;
  case 0xd:
    UiCollectionMenuHideCursor(self);
    UiCollectionMenuStartItemBoxZoom(self, 1);
    UiCollectionMenuStartTitleFade(self, 1);
    UiCollectionMenuStartButtonZoom(self, 1);
    UiCollectionMenuStartFrameTween(self, 1);
    self->base.phaseStep++;
    break;
  case 0xe:
    done = UiCollectionMenuUpdateItemBoxZoom(self, 1);
    done += UiCollectionMenuUpdateTitleFade(self, 1);
    done += UiCollectionMenuUpdateButtonZoom(self, 1);
    done += UiCollectionMenuUpdateFrameTween(self, 1);
    if (done == 4) {
      UiCollectionMenuReleaseItemBox(self);
      self->base.phaseStep = 0xf;
    }
    break;
  case 0xf:
    /* Open the collection screen of the selected main entry (phase 3..7). */
    sel = self->selMain;
    switch (sel) {
    case 0:
      self->base.phase = 3;
      self->base.phaseStep = 0;
      break;
    case 1:
      self->base.phase = 4;
      self->base.phaseStep = 0;
      break;
    case 2:
      self->base.phase = 5;
      self->base.phaseStep = 0;
      break;
    case 3:
      self->base.phase = 6;
      self->base.phaseStep = 0;
      break;
    case 4:
      self->base.phase = 7;
      self->base.phaseStep = 0;
      break;
    }
    break;
  case 0x10:
    UiCollectionMenuLoadItemBox(self);
    UiCollectionMenuClearItemBoxLights(self);
    UiCollectionMenuStartItemBoxZoom(self, 0);
    UiCollectionMenuStartTitleFade(self, 0);
    UiCollectionMenuStartButtonZoom(self, 0);
    UiCollectionMenuStartFrameTween(self, 0);
    UiCollectionMenuRestoreItemBoxMotion(self, 1);
    UiCollectionMenuStartLightBlink(self);
    self->base.phaseStep++;
    break;
  case 0x11:
    done = UiCollectionMenuUpdateItemBoxZoom(self, 0);
    done += UiCollectionMenuUpdateTitleFade(self, 0);
    done += UiCollectionMenuUpdateButtonZoom(self, 0);
    done += UiCollectionMenuUpdateFrameTween(self, 0);
    if (done == 4) {
      UiCollectionMenuResetCursor(self);
      self->base.phaseStep = (self->page == 0) ? 2 : 10;
    }
    break;
  default:
    /* Sub-state 0x12 (after the close fade) or any other: exit with result 0. */
    UiCollectionMenuSetResultNone(self);
    UiSharedBgClose();
    g_uiKeepSharedBg = 0;
    self->base.phase = 8;
    self->base.phaseStep = 0;
    break;
  }
  UiCollectionMenuUpdateLightBlink(self);
}
