// bdc 0x089a213c UiWorldMapPhaseMain
#include "bdc.h"

/* Phase 2 of `UiWorldMap` (phase table `0x08a9eeb8` entry 2): the area selection.
   Steps 0/1 slide everything in (`"main_light.fab"`, `UiWorldMapStartAreaButtonsTween`,
   `UiWorldMapStartGlobeTween`, `UiWorldMapStartPreviewTween`, `UiWorldMapStartJetFlyIn` and
   their *Done* checks) and show the button guide; step 3 is the idle loop: cursor pulse/zoom,
   confirm (`UiWorldMapCheckConfirm` → flash → step 10), cancel (→ 4), options (square,
   `UiWorldMapCheckOptionsButton` → phase 5), random pick (triangle,
   `UiWorldMapCheckRandomButton` → `UiWorldMapAreaRoulette`) and up/down
   (`UiWorldMapMoveCursor`). After the flash, story mode asks for confirmation
   (`UiWorldMapShowConfirm`) while rank mode opens the stage list (steps 0x11..0x19:
   `UiWorldMapStartAreaListFade`, `UiWorldMapStartStageListTween`, stage cursor,
   `UiWorldMapStageRoulette`, confirm/cancel). Leaving (steps 4..9, 0x1a/0x1b) flies the jet out
   (`UiWorldMapJetFlyDone`), slides everything out with `"main_finish.fab"`, stops the BGM when
   needed and fades to black; the final step sets the menu result (`UiWorldMapSetResult`), closes
   the shared background and advances the phase. Every frame it updates the globe, area labels and
   jet pose (`UiWorldMapUpdateGlobe`, `UiWorldMapPlaceAreaLabels`, `UiWorldMapUpdateJetPose`).
    */

void UiWorldMapPhaseMain(UiScreen *screen)
{
  UiWorldMap *map = (UiWorldMap *)screen;
  GfxSprite **sprites;
  GfxFader *fader;
  u32 frame;
  u8 done;
  u8 result;

  switch (screen->phaseStep) {
  case 0:
    frame = UiSharedAnimGetFrame(screen, 1);
    if (frame == UiSharedAnimGetLength(screen, 1)) {
      UiSharedAnimRelease(screen, 1);
      UiSharedAnimStart(100.0f, 0.0f, 0.0f, screen, (void *)"main_light.fab", 1, 1);
      GfxFabUpdate(g_uiSharedAnims[1]);
      GfxFabUpdate(g_uiSharedAnims[1]);
      sprites = (GfxSprite **)screen->data;
      UiTitlePlateInit(0, sprites[0x19]);
      UiWorldMapStartAreaButtonsTween(screen, 0);
      UiWorldMapStartGlobeTween(screen, 0);
      UiWorldMapStartPreviewTween(screen, 0);
      UiWorldMapStartJetFlyIn(screen, 0);
      screen->phaseStep++;
    }
    break;
  case 1:
    done = UiTitlePlateStep(0);
    done += UiWorldMapAreaButtonsDone(screen, 0);
    done += UiWorldMapPreviewDone(screen, 0);
    done += UiWorldMapGlobeTweenDone(screen, 0);
    done += UiWorldMapJetFlyDone(screen, 0);
    if (done == 5) {
      UiWorldMapShowButtonGuide(screen, 1);
      screen->phaseStep++;
    }
    break;
  case 2:
    UiWorldMapRefreshAreaCursor(screen);
    map->jetActive = 1;
    screen->phaseStep++;
    break;
  case 3:
    UiWorldMapPulseCursor(screen);
    UiWorldMapPulseSelectedArea(screen);
    UiWorldMapZoomSelectedArea(screen);
    sprites = (GfxSprite **)screen->data;
    UiPulseStep(sprites[0x5d], &map->randomPulse);
    result = (u8)UiWorldMapCheckConfirm(screen);
    if (result != 0) {
      if (result == 1) {
        if (SndHasManager()) {
          SndManagerPlay(SndGetManager(), 0, 0, 0);
        }
        UiWorldMapRefreshAreaCursor(screen);
        UiWorldMapStartSelectFlash(screen);
        map->cancelFlag = 0;
        screen->phaseStep = 10;
      } else if (SndHasManager()) {
        SndManagerPlay(SndGetManager(), 3, 0, 0);
      }
      break;
    }
    if (screen->pad->pressed & 0x2000) {
      if (SndHasManager()) {
        SndManagerPlay(SndGetManager(), 2, 0, 0);
      }
      UiWorldMapRefreshAreaCursor(screen);
      map->cancelFlag = 1;
      screen->phaseStep = 4;
      break;
    }
    if (UiWorldMapCheckOptionsButton(screen) != 0) {
      if (SndHasManager()) {
        SndManagerPlay(SndGetManager(), 0, 0, 0);
      }
      UiWorldMapRefreshAreaCursor(screen);
      UiWorldMapHideCursor(screen);
      UiWorldMapShowButtonGuide(screen, 0);
      screen->phase = 5;
      screen->phaseStep = 0;
      break;
    }
    result = (u8)UiWorldMapCheckRandomButton(screen);
    if (result != 0) {
      if (result == 1) {
        UiWorldMapRefreshAreaCursor(screen);
        map->cancelFlag = 0;
        screen->phaseStep = 0xc;
      } else if (SndHasManager()) {
        SndManagerPlay(SndGetManager(), 3, 0, 0);
      }
      break;
    }
    if (UiWorldMapMoveCursor(screen) == 1) {
      if (SndHasManager()) {
        SndManagerPlay(SndGetManager(), 1, 0, 0);
      }
      UiWorldMapRefreshAreaCursor(screen);
    }
    break;
  case 4:
    UiWorldMapShowButtonGuide(screen, 0);
    UiWorldMapHideCursor(screen);
    map->jetActive = 0;
    map->ringsHidden = 1;
    UiWorldMapStartJetFlyIn(screen, 1);
    screen->phaseStep++;
    break;
  case 5:
    if (UiWorldMapJetFlyDone(screen, 1) == 1) {
      screen->phaseStep = 6;
    }
    break;
  case 6:
    UiWorldMapHideCursor(screen);
    sprites = (GfxSprite **)screen->data;
    UiTitlePlateInit(1, sprites[0x19]);
    UiWorldMapStartAreaButtonsTween(screen, 1);
    UiWorldMapStartPreviewTween(screen, 1);
    UiWorldMapStartGlobeTween(screen, 1);
    UiSharedAnimRelease(screen, 1);
    UiSharedAnimStart(100.0f, 0.0f, 0.0f, screen, (void *)"main_finish.fab", 1, 0);
    GfxFabUpdate(g_uiSharedAnims[1]);
    GfxFabUpdate(g_uiSharedAnims[1]);
    screen->phaseStep++;
    break;
  case 7:
    done = UiTitlePlateStep(1);
    done += UiWorldMapAreaButtonsDone(screen, 1);
    done += UiWorldMapPreviewDone(screen, 1);
    done += UiWorldMapGlobeTweenDone(screen, 1);
    if (done == 4) {
      screen->phaseStep = 8;
    }
    break;
  case 8:
    frame = UiSharedAnimGetFrame(screen, 1);
    if ((s32)frame < (s32)UiSharedAnimGetLength(screen, 1)) {
      break;
    }
    if (map->cancelFlag == 0 || UiWorldMapIsRankMode(screen) == 1) {
      SndBgmCancelChannel(0);
      SndBgmQueueStop(1.0f, 0);
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
    GfxFaderStart(GfxGetActiveFader(), 0x10);
    screen->phaseStep++;
    break;
  case 9:
    if (GfxFaderIsFinished(GfxGetActiveFader())) {
      screen->phaseStep = 0x1c;
    }
    break;
  case 10:
    if (UiWorldMapSelectFlashDone(screen) == 1) {
      if (!UiWorldMapIsRankMode(screen)) {
        UiWorldMapResetDialog(screen, 0);
        screen->phaseStep = 0xb;
      } else {
        screen->phaseStep = 0x11;
      }
    }
    break;
  case 0xb:
    if (UiWorldMapShowConfirm(screen) == 1) {
      if ((s8)map->dialogState[0] == 0) {
        UiWorldMapClearNewMarker(screen);
        screen->phaseStep = 4;
      } else {
        screen->phaseStep = 3;
      }
    }
    break;
  case 0xc:
    UiWorldMapResetPopupState(screen);
    screen->phaseStep++;
    break;
  case 0xd:
    if (UiWorldMapAreaRoulette(screen) == 1) {
      if (SndHasManager()) {
        SndManagerPlay(SndGetManager(), 0, 0, 0);
      }
      UiWorldMapRefreshAreaCursor(screen);
      UiWorldMapStartSelectFlash(screen);
      map->cancelFlag = 0;
      screen->phaseStep = 10;
    }
    break;
  case 0xe:
    if (UiWorldMapGlobeTurnStep(screen) != 0) {
      screen->phaseStep = 0xf;
    }
    break;
  case 0xf:
    UiWorldMapDebugRotateGlobe(screen);
    if (screen->pad->pressed & 8) {
      map->shownAreaId = -1;
      screen->phaseStep = 3;
    }
    break;
  case 0x10:
    UiWorldMapDebugRotateJet(screen);
    if (screen->pad->repeat & 0x8000) {
      screen->phaseStep = 3;
    }
    break;
  case 0x11:
    UiWorldMapHideCursor(screen);
    UiWorldMapResetStage(map);
    UiWorldMapStartAreaListFade(screen, 1);
    UiWorldMapStartStageListTween(screen, 0);
    screen->phaseStep++;
    break;
  case 0x12:
    done = UiWorldMapAreaListFadeDone(screen, 1);
    done += UiWorldMapStageListDone(screen, 0);
    if (done == 2) {
      UiWorldMapCacheScoreOffsets(screen);
      UiWorldMapRefreshStageCursor(screen);
      screen->phaseStep = 0x13;
    }
    break;
  case 0x13:
    UiWorldMapPulseStageCursor(screen);
    UiWorldMapZoomSelectedStage(screen);
    sprites = (GfxSprite **)screen->data;
    UiPulseStep(sprites[0x5d], &map->randomPulse);
    result = (u8)UiWorldMapCheckStageConfirm(screen);
    if (result != 0) {
      if (result == 1) {
        if (SndHasManager()) {
          SndManagerPlay(SndGetManager(), 0, 0, 0);
        }
        UiWorldMapRefreshStageCursor(screen);
        UiWorldMapStartStageFlash(screen);
        map->cancelFlag = 0;
        screen->phaseStep = 0x16;
      } else if (SndHasManager()) {
        SndManagerPlay(SndGetManager(), 3, 0, 0);
      }
      break;
    }
    if (screen->pad->pressed & 0x2000) {
      if (SndHasManager()) {
        SndManagerPlay(SndGetManager(), 2, 0, 0);
      }
      UiWorldMapRefreshStageCursor(screen);
      UiWorldMapHideStageCursor(screen);
      map->cancelFlag = 1;
      screen->phaseStep = 0x14;
      break;
    }
    result = (u8)UiWorldMapCheckStageRandomButton(screen);
    if (result == 1) {
      if (screen->pad->pressed & 0x1000) {
        UiWorldMapRefreshStageCursor(screen);
        map->cancelFlag = 0;
        screen->phaseStep = 0x18;
        break;
      }
    } else if (result != 0) {
      if (SndHasManager()) {
        SndManagerPlay(SndGetManager(), 3, 0, 0);
      }
    }
    if (UiWorldMapMoveStageCursor(screen) == 1) {
      if (SndHasManager()) {
        SndManagerPlay(SndGetManager(), 1, 0, 0);
      }
      UiWorldMapRefreshStageCursor(screen);
    }
    break;
  case 0x14:
    UiWorldMapStartAreaListFade(screen, 0);
    UiWorldMapStartStageListTween(screen, 1);
    screen->phaseStep++;
    break;
  case 0x15:
    done = UiWorldMapAreaListFadeDone(screen, 0);
    done += UiWorldMapStageListDone(screen, 1);
    if (done == 2) {
      UiWorldMapRefreshAreaCursor(screen);
      map->stage = 0;
      UiWorldMapShowStagePreview(screen, (u8)map->stage);
      screen->phaseStep = 3;
    }
    break;
  case 0x16:
    if (UiWorldMapSelectFlashDone(screen) == 1) {
      UiWorldMapResetDialog(screen, 0);
      screen->phaseStep = 0x17;
    }
    break;
  case 0x17:
    if (UiWorldMapShowConfirm(screen) == 1) {
      screen->phaseStep = ((s8)map->dialogState[0] == 0) ? 0x1a : 0x13;
    }
    break;
  case 0x18:
    UiWorldMapResetPopupState(screen);
    screen->phaseStep++;
    break;
  case 0x19:
    if (UiWorldMapStageRoulette(screen) == 1) {
      if (SndHasManager()) {
        SndManagerPlay(SndGetManager(), 0, 0, 0);
      }
      UiWorldMapRefreshStageCursor(screen);
      UiWorldMapStartStageFlash(screen);
      map->cancelFlag = 0;
      screen->phaseStep = 0x16;
    }
    break;
  case 0x1a:
    UiWorldMapShowButtonGuide(screen, 0);
    UiWorldMapHideStageCursor(screen);
    sprites = (GfxSprite **)screen->data;
    UiTitlePlateInit(1, sprites[0x19]);
    UiWorldMapStartPreviewTween(screen, 1);
    UiWorldMapStartGlobeTween(screen, 1);
    UiWorldMapStartStageListTween(screen, 1);
    UiSharedAnimRelease(screen, 1);
    UiSharedAnimStart(100.0f, 0.0f, 0.0f, screen, (void *)"main_finish.fab", 1, 0);
    GfxFabUpdate(g_uiSharedAnims[1]);
    GfxFabUpdate(g_uiSharedAnims[1]);
    screen->phaseStep++;
    break;
  case 0x1b:
    done = UiTitlePlateStep(1);
    done += UiWorldMapPreviewDone(screen, 1);
    done += UiWorldMapGlobeTweenDone(screen, 1);
    done += UiWorldMapStageListDone(screen, 1);
    if (done == 4) {
      screen->phaseStep = 8;
    }
    break;
  default:
    UiWorldMapSetResult(screen);
    UiSharedBgClose();
    g_uiKeepSharedBg = 0;
    screen->phaseStep = 0;
    screen->phase++;
    break;
  }
  UiWorldMapUpdateGlobe(screen);
  UiWorldMapPlaceAreaLabels(screen);
  UiWorldMapUpdateJetPose(screen);
}
