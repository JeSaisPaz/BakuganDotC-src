// bdc 0x0898a5cc UiCollectionTheaterMainPhase
#include "bdc.h"

/* Main phase (entry 2 of the phase table `0x08a9e928`) of the theater collection screen (task 315,
   `maybe_UiScreen315Ctor`; replays unlocked scenes: buttons `"collection_scene_%02d"` in
   `"waku_4_a"`/`"waku_4_b"` frames, thumbnails `"cinema_%02d"`; page `+0x8e1`, selection `+0x8e0`):
   opens all parts, then handles cursor movement (`UiCollectionTheaterMoveCursor`), page change (`UiCollectionTheaterChangePage` →
   sub-states 6..9 with `UiCollectionTheaterAnimatePageArrow`), confirm (`UiCollectionTheaterCheckConfirm` →
   `UiCollectionTheaterStartCellPress` plays the scene, sub-state 5 waits with `UiCollectionTheaterWaitPress`, then BGM stop,
   fade to black and phase 3) and cancel (Circle: sound 2, `cancelled` = 1, close tweens, sub-state 0xf → phase 4 with
   result 0). Sub-states 0xd/0xe fade back in with BGM 0x16 and return to sub-state 2. Every frame ends with
   `UiCollectionTheaterUpdateThumbBob`. */

void UiCollectionTheaterMainPhase(UiScreen *screen)

{
  UiCollectionTheater *self = (UiCollectionTheater *)screen;
  GfxSprite **sprites;
  GfxFader *fader;
  u8 done;
  int confirm;

  switch (screen->phaseStep) {
  case 0:
    UiCollectionTheaterStartBgTween(screen, 0);
    UiCollectionTheaterStartSceneButtonTween(screen, 0);
    UiCollectionTheaterStartFrameTween(screen, 0);
    UiCollectionTheaterStartArrowTween(screen, 0);
    UiCollectionTheaterStartThumbnailTween(screen, 0);
    screen->phaseStep = screen->phaseStep + 1;
    break;
  case 1:
    done = UiCollectionTheaterUpdateBgTween(self, 0);
    done = (u8)(done + UiCollectionTheaterUpdateSceneButtonTween(screen, 0));
    done = (u8)(done + UiCollectionTheaterUpdateFrameTween(screen, 0));
    done = (u8)(done + UiCollectionTheaterUpdateArrowTween(self, 0));
    done = (u8)(done + UiCollectionTheaterUpdateThumbnailTween(screen, 0));
    if (done == 5) {
      UiCollectionTheaterEnableThumbBob(screen, 1);
      UiCollectionTheaterResetCursor(screen);
      screen->phaseStep = 2;
    }
    break;
  case 2:
    UiCollectionTheaterPulseCursor(screen);
    UiCollectionTheaterAnimateCell(screen);
    UiCollectionTheaterZoomCursorCell(screen);
    sprites = (GfxSprite **)screen->data;
    UiPulseStep(sprites[48], &self->ghostPulse);
    confirm = (u8)UiCollectionTheaterCheckConfirm(screen);
    if (confirm == 0) {
      if ((screen->pad->pressed & 0x2000) != 0) {
        /* Circle: leave the screen */
        if (SndHasManager()) {
          SndManagerPlay(SndGetManager(), 2, 0, 0);
        }
        self->cancelled = 1;
        UiCollectionTheaterEnableThumbBob(screen, 0);
        UiCollectionTheaterHideCursor(screen);
        screen->phaseStep = 3;
      }
      else if (UiCollectionTheaterMoveCursor(screen) == 1) {
        if (SndHasManager()) {
          SndManagerPlay(SndGetManager(), 1, 0, 0);
        }
        UiCollectionTheaterResetCursor(screen);
      }
      else if (UiCollectionTheaterChangePage(screen) == 1) {
        if (SndHasManager()) {
          SndManagerPlay(SndGetManager(), 1, 0, 0);
        }
        UiCollectionTheaterResetCursor(screen);
        UiCollectionTheaterHideCursor(screen);
        UiCollectionTheaterEnableThumbBob(screen, 0);
        screen->phaseStep = 6;
      }
    }
    else if (confirm == 1) {
      /* unlocked scene chosen */
      if (SndHasManager()) {
        SndManagerPlay(SndGetManager(), 0, 0, 0);
      }
      UiCollectionTheaterEnableThumbBob(screen, 0);
      UiCollectionTheaterResetCursor(screen);
      UiCollectionTheaterHideCursor(screen);
      UiCollectionTheaterEnableThumbBob(screen, 0);
      UiCollectionTheaterStartCellPress(screen);
      self->cancelled = 0;
      screen->phaseStep = 5;
    }
    else {
      /* locked slot */
      if (SndHasManager()) {
        SndManagerPlay(SndGetManager(), 3, 0, 0);
      }
    }
    break;
  case 3:
    UiCollectionTheaterStartBgTween(screen, 1);
    UiCollectionTheaterStartSceneButtonTween(screen, 1);
    UiCollectionTheaterStartFrameTween(screen, 1);
    UiCollectionTheaterStartArrowTween(screen, 1);
    UiCollectionTheaterStartThumbnailTween(screen, 1);
    screen->phaseStep = screen->phaseStep + 1;
    break;
  case 4:
    done = UiCollectionTheaterUpdateBgTween(self, 1);
    done = (u8)(done + UiCollectionTheaterUpdateSceneButtonTween(screen, 1));
    done = (u8)(done + UiCollectionTheaterUpdateFrameTween(screen, 1));
    done = (u8)(done + UiCollectionTheaterUpdateArrowTween(self, 1));
    done = (u8)(done + UiCollectionTheaterUpdateThumbnailTween(screen, 1));
    if (done == 5) {
      screen->phaseStep = 0xf;
    }
    break;
  case 5:
    if (UiCollectionTheaterWaitPress(screen) == 1) {
      screen->phaseStep = 10;
    }
    break;
  case 6:
    UiCollectionTheaterStartSceneButtonTween(screen, 1);
    UiCollectionTheaterStartThumbnailTween(screen, 1);
    UiCollectionTheaterStartPageArrowPress(screen);
    screen->phaseStep = screen->phaseStep + 1;
    break;
  case 7:
    UiCollectionTheaterAnimatePageArrow(screen);
    done = UiCollectionTheaterUpdateSceneButtonTween(screen, 1);
    done = (u8)(done + UiCollectionTheaterUpdateThumbnailTween(screen, 1));
    if (done == 2) {
      self->page = self->targetPage;
      UiCollectionTheaterSetPageNumber(screen);
      UiCollectionTheaterClampCursorToPage(screen);
      screen->phaseStep = screen->phaseStep + 1;
    }
    break;
  case 8:
    UiCollectionTheaterAnimatePageArrow(screen);
    UiCollectionTheaterStartSceneButtonTween(screen, 0);
    UiCollectionTheaterStartThumbnailTween(screen, 0);
    screen->phaseStep = screen->phaseStep + 1;
    break;
  case 9:
    done = (u8)UiCollectionTheaterAnimatePageArrow(screen);
    done = (u8)(done + UiCollectionTheaterUpdateSceneButtonTween(screen, 0));
    done = (u8)(done + UiCollectionTheaterUpdateThumbnailTween(screen, 0));
    if (done == 3) {
      self->pageDir = 0;
      UiCollectionTheaterResetCursor(screen);
      UiCollectionTheaterEnableThumbBob(screen, 1);
      UiCollectionTheaterDimPageArrows(1.0f, screen);
      screen->phaseStep = 2;
    }
    break;
  case 10:
    /* scene chosen: stop the BGM and fade to black */
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
    screen->phaseStep = screen->phaseStep + 1;
    break;
  case 11:
    if (GfxFaderIsFinished(GfxGetActiveFader())) {
      screen->phaseStep = screen->phaseStep + 1;
    }
    break;
  case 12:
    screen->phase = 3;
    screen->phaseStep = 0;
    break;
  case 13:
    /* back from the scene: restart the menu BGM and fade in */
    SndBgmQueuePlay(0, 0x16, 1, 0);
    fader = GfxGetActiveFader();
    fader->start[0] = 0.0f;
    fader->start[1] = 0.0f;
    fader->start[2] = 0.0f;
    fader->start[3] = 1.0f;
    fader = GfxGetActiveFader();
    fader->end[0] = 0.0f;
    fader->end[1] = 0.0f;
    fader->end[2] = 0.0f;
    fader->end[3] = 0.0f;
    GfxFaderStart(GfxGetActiveFader(), 0x10);
    screen->phaseStep = screen->phaseStep + 1;
    break;
  case 14:
    if (GfxFaderIsFinished(GfxGetActiveFader())) {
      UiCollectionTheaterResetCursor(screen);
      UiCollectionTheaterEnableThumbBob(screen, 1);
      screen->phaseStep = 2;
    }
    break;
  default:
    UiCollectionTheaterSetResultNone(screen);
    screen->phase = 4;
    screen->phaseStep = 0;
    break;
  }
  UiCollectionTheaterUpdateThumbBob(screen);
  return;
}
