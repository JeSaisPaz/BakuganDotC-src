// bdc 0x089a2dbc UiWorldMapNetPhaseMain
#include "bdc.h"

/* Phase 4 of `UiWorldMap`, the ad-hoc (net) version of `UiWorldMapPhaseMain`:
   when profile flag 0 is set and a NetPlay manager exists, each frame of a non-local step reads the
   peer's record (`NetCharaReadSlot` of slot `netSession == 0` → `UiWorldMapApplyNetState` when
   it carries flag `0x2000000`) and pushes the local record `player[netSession]` with `{step,
   cancelFlag}` (`NetCharaPushMessage`); the step machine only runs on frames where a record was
   read (or the step is local-only, the handshake is done, or there is no net session). With profile
   flag 0x80 at step 3 or 0x1d it opens advisor message 0x1d and waits for it in step 0x1e. The step
   machine is the single-player one (`main_light.fab`, area cursor, confirm/random/options buttons,
   area roulette, `main_finish.fab`, fade out) with extra steps 0x1d/0x1f that wait until both
   players reached the same step (`player[0/1].cursorA`); cancel also sets NetPlay flag
   `0x1000000`. Every frame it updates the globe, area labels and jet pose and records the step in
   `g_uiWorldMapNetLastStep`. */

void UiWorldMapNetPhaseMain(UiScreen *screen)
{
  UiWorldMap *map = (UiWorldMap *)screen;
  GfxSprite **sprites = (GfxSprite **)screen->data;
  bool proceed;
  NetChara *chara;
  NetCharaMsg msg;
  s32 slot;
  s32 lp;
  u32 flags;
  u8 n;
  u8 r;
  GfxFader *fader;
  s16 peerStep;
  s32 i;

  proceed = true;
  if (SaveGetProfileFlag0()) {
    if (NetPlayHasManager()) {
      proceed = false;
      if (NetPlayHasManager()) {
        switch (screen->phaseStep) {
        case 0: case 1: case 2: case 4: case 5: case 6: case 7: case 8: case 9:
        case 0x1c: case 0x1e:
          proceed = true;
          break;
        default:
          break;
        }
        if (!proceed) {
          chara = NetCharaGetByIndex(0);
          if ((NetPlayGetFlags(NetPlayGetManager()) & 0x2000000) != 0 &&
              NetCharaIsSyncHandshakeDone()) {
            proceed = true;
          }
          if (!proceed && chara != NULL) {
            if (NetPlayIsSynced(NetPlayGetManager())) {
              slot = (map->netSession == 0);
              if (NetCharaReadSlot(chara, slot, (u32 *)&msg)) {
                proceed = true;
                if (msg.flags & 0x2000000) {
                  UiWorldMapApplyNetState(screen, slot, (s16 *)msg.body.raw);
                  proceed = true;
                }
              }
            }
            memset(&msg, 0, sizeof(msg));
            lp = map->netSession;
            msg.flags = 0x2000000;
            map->player[lp].cursorA = (s16)screen->phaseStep;
            map->player[lp].cursorB = map->cancelFlag;
            memcpy(msg.body.raw, &map->player[lp], 0x10);
            NetCharaPushMessage(chara, (u32 *)&msg);
          }
        }
      }
    }
    if (SaveHasProfile() && SaveProfileHasFlags(SaveGetProfile(), 0x80) &&
        (screen->phaseStep == 3 || screen->phaseStep == 0x1d)) {
      if (!UiMsgWindowExists()) {
        UiMsgWindowEnsure();
      }
      ((UiMsgWindow *)UiMsgWindowGet())->mode = 1;
      UiMsgWindowOpen((UiMsgWindow *)UiMsgWindowGet(), 0, 0x1d);
      screen->phaseStep = 0x1e;
    }
  }

  if (proceed) {
    switch (screen->phaseStep) {
    case 0:
      if (UiSharedAnimIsDone(screen, 1, 0)) {
        UiSharedAnimRelease(screen, 1);
        UiSharedAnimStart(100.0f, 0.0f, 0.0f, screen, (void *)"main_light.fab", 1, 1);
        GfxFabUpdate(g_uiSharedAnims[1]);
        GfxFabUpdate(g_uiSharedAnims[1]);
        UiTitlePlateInit(0, sprites[0x19]);
        UiWorldMapStartAreaButtonsTween(screen, 0);
        UiWorldMapStartGlobeTween(screen, 0);
        UiWorldMapStartPreviewTween(screen, 0);
        UiWorldMapStartJetFlyIn(screen, 0);
        screen->phaseStep++;
      }
      break;
    case 1:
      n = UiTitlePlateStep(0);
      n += UiWorldMapAreaButtonsDone(screen, 0);
      n += UiWorldMapPreviewDone(screen, 0);
      n += UiWorldMapGlobeTweenDone(screen, 0);
      n += UiWorldMapJetFlyDone(screen, 0);
      if (n == 5) {
        UiWorldMapShowButtonGuide(screen, 1);
        screen->phaseStep++;
      }
      break;
    case 2:
      UiWorldMapRefreshAreaCursor(screen);
      map->jetActive = 1;
      screen->phaseStep = (map->netSession == 0) ? 3 : 0x1d;
      break;
    case 3:
      UiWorldMapPulseCursor(screen);
      UiWorldMapPulseSelectedArea(screen);
      UiWorldMapZoomSelectedArea(screen);
      UiPulseStep(sprites[0x5d], &map->randomPulse);
      r = (u8)UiWorldMapCheckConfirm(screen);
      if (r != 0) {
        if (r == 1) {
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
      } else if (screen->pad->pressed & 0x2000) {
        if (SndHasManager()) {
          SndManagerPlay(SndGetManager(), 2, 0, 0);
        }
        UiWorldMapRefreshAreaCursor(screen);
        map->cancelFlag = 1;
        screen->phaseStep = 0x1d;
      } else if (UiWorldMapCheckOptionsButton(screen) != 0) {
        if (SndHasManager()) {
          SndManagerPlay(SndGetManager(), 0, 0, 0);
        }
        UiWorldMapRefreshAreaCursor(screen);
        UiWorldMapHideCursor(screen);
        UiWorldMapShowButtonGuide(screen, 0);
        screen->phaseStep = 0x1f;
      } else {
        r = (u8)UiWorldMapCheckRandomButton(screen);
        if (r != 0) {
          if (r == 1) {
            UiWorldMapRefreshAreaCursor(screen);
            map->cancelFlag = 0;
            screen->phaseStep = 0xc;
          } else if (SndHasManager()) {
            SndManagerPlay(SndGetManager(), 3, 0, 0);
          }
        } else if (UiWorldMapMoveCursor(screen) == 1) {
          if (SndHasManager()) {
            SndManagerPlay(SndGetManager(), 1, 0, 0);
          }
          UiWorldMapRefreshAreaCursor(screen);
          UiWorldMapSetPlayerSelection(screen, map->netSession, map->areaId);
        }
      }
      break;
    case 4:
      if (NetPlayHasManager() && map->cancelFlag == 0) {
        flags = NetPlayGetFlags(NetPlayGetManager());
        NetPlaySetFlags(NetPlayGetManager(), flags | 0x1000000);
      }
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
      n = UiTitlePlateStep(1);
      n += UiWorldMapAreaButtonsDone(screen, 1);
      n += UiWorldMapPreviewDone(screen, 1);
      n += UiWorldMapGlobeTweenDone(screen, 1);
      if (n == 4) {
        screen->phaseStep = 8;
      }
      break;
    case 8:
      if (UiSharedAnimIsDone(screen, 1, 0)) {
        if (map->cancelFlag == 0) {
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
      }
      break;
    case 9:
      if (GfxFaderIsFinished(GfxGetActiveFader())) {
        screen->phaseStep = 0x1c;
      }
      break;
    case 10:
      if (UiWorldMapSelectFlashDone(screen) == 1) {
        UiWorldMapResetDialog(screen, 0);
        screen->phaseStep = 0xb;
      }
      break;
    case 0xb:
      if (UiWorldMapShowConfirm(screen) == 1) {
        if ((s8)map->dialogState[0] != 0) {
          screen->phaseStep = 3;
        } else {
          UiWorldMapClearNewMarker(screen);
          screen->phaseStep = 0x1d;
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
      if (screen->pad->repeat & 0x1000) {
        screen->phaseStep = 3;
      }
      break;
    case 0x1d:
      /* wait until both records reach 0x1d; in a session follow player 0 into options (0x1f) */
      peerStep = map->player[0].cursorA;
      if (map->netSession != 0 && peerStep == 0x1f) {
        screen->phaseStep = 0x1f;
      } else if (peerStep == 0x1d && map->player[1].cursorA == 0x1d) {
        screen->phaseStep = 4;
      }
      break;
    case 0x1e:
      proceed = true;
      if (UiMsgWindowExists()) {
        proceed = false;
        if (UiMsgWindowIsClosed((UiMsgWindow *)UiMsgWindowGet())) {
          proceed = true;
        }
      }
      if (proceed) {
        screen->phaseStep = 4;
      }
      break;
    case 0x1f:
      if (map->player[0].cursorA == 0x1f && map->player[1].cursorA == 0x1f) {
        UiWorldMapShowButtonGuide(screen, 0);
        screen->phase = 5;
        screen->phaseStep = 0;
      }
      break;
    default: /* 0x11..0x1c and out of range: finish */
      UiWorldMapSetResult(screen);
      UiSharedBgClose();
      g_uiKeepSharedBg = 0;
      screen->phase = 3;
      screen->phaseStep = 0;
      break;
    }
  }

  UiWorldMapUpdateGlobe(screen);
  UiWorldMapPlaceAreaLabels(screen);
  UiWorldMapUpdateJetPose(screen);
  if (screen->phaseStep != g_uiWorldMapNetLastStep) {
    g_uiWorldMapNetLastStep = screen->phaseStep;
  }
  for (i = 0; i < 2; i++) {
    /* empty loop left in the binary (debug output compiled out) */
  }
}
