// bdc 0x089677b8 UiEquipNetMainPhase
#include "bdc.h"

/* Network main phase (entry 5 of the phase table `0x08a9d5f8`) of the Bakugan/gear loadout screen
   before a battle (task 302, `maybe_UiScreen302Ctor`; per player picks Bakugan from a 20-face
   grid (`"baku_face_%02d"`) and equipment (`"c_setting_soubi_l_sol_%02d"`), shown on
   `"menu_daiza.gmo"` pedestals; `+0x4cda` = number of players, `+0x4cdb` = player being edited):
   the two-player version of `UiEquipMainPhase`. Outside the intro/outro steps (0-4, 7-10, 23,
   25), while a NetPlay manager exists and the sync handshake is not done, it reads the peer's
   record from slot `localPlayer == 0` of net character 0 (`NetCharaReadSlot`, applied with
   `UiEquipNetReceivePeerRecord` when flag 0x2000000 is set) and pushes the local one
   (`NetCharaPushMessage`: Bakugan, equipment, handicap, phaseStep, gearEditing); on a frame
   without a record it returns at once. Then: intro (`"main_light.fab"`), grid pick (step 5),
   random pick, equipment panel (steps 14-20), waiting for the peer (step 24), and the outro
   (`"main_finish.fab"`, fade, phase 4 after `UiEquipSaveSelections` and `UiSharedBgClose`).
   A lost connection (profile flag 0x80) shows message 29 and leaves; Circle in step 5 asks
   whether to quit (message 26, step 26); when the peer reports it is leaving (`extra14` = 1) the
   session is aborted (`NetPlayRequestAbort`) after message 30/27. In the selection steps it
   applies the peer selections (`UiEquipApplyPeerSelections`) every frame. */

void UiEquipNetMainPhase(UiEquip *self)
{
  bool proceed;
  NetChara *chara;
  NetCharaMsg msg;
  s32 slot;
  s32 lp;
  s32 r;
  u8 n;

  UiEquipUpdateAnimations(self);
  proceed = true;
  if (NetPlayHasManager()) {
    proceed = false;
    if (NetPlayHasManager()) {
      switch (self->base.phaseStep) {
      case 0: case 1: case 2: case 3: case 4:
      case 7: case 8: case 9: case 10:
      case 23: case 25:
        proceed = true;
        break;
      default:
        break;
      }
      if (!proceed) {
        chara = NetCharaGetByIndex(0);
        if (NetCharaIsSyncHandshakeDone()) {
          proceed = true;
        }
        if (!proceed) {
          if (chara == NULL) {
            return;
          }
          if (NetPlayIsSynced(NetPlayGetManager())) {
            slot = (*(s32 *)self->localPlayer == 0);
            if (NetCharaReadSlot(chara, slot, (u32 *)&msg)) {
              proceed = true;
              if (msg.flags & 0x2000000) {
                UiEquipNetReceivePeerRecord(self, slot, (const u32 *)msg.body.raw);
                proceed = true;
              }
            }
          }
          memset(&msg, 0, sizeof(msg));
          lp = *(s32 *)self->localPlayer;
          msg.flags = 0x2000000;
          self->peer[lp].extra10 = self->base.phaseStep;
          self->peer[lp].extra14 = self->gearEditing;
          memcpy(msg.body.raw, &self->peer[lp], 0x18);
          NetCharaPushMessage(chara, (u32 *)&msg);
        }
      }
    }
  }
  if (!proceed) {
    return;
  }

  if (SaveHasProfile() && SaveProfileHasFlags(SaveGetProfile(), 0x80)) {
    r = self->base.phaseStep;
    if (r == 0x18 || r == 0x10 || r == 5) {
      if (!UiMsgWindowExists()) {
        UiMsgWindowEnsure();
      }
      ((UiMsgWindow *)UiMsgWindowGet())->mode = 1;
      UiMsgWindowOpen((UiMsgWindow *)UiMsgWindowGet(), 0, 0x1d);
      self->base.phaseStep = 0x19;
    }
  }

  switch (self->base.phaseStep) {
  case 0:
    if (UiSharedAnimIsDone(self, (u32)-1, 1)) {
      UiSharedAnimRelease(self, 1);
      UiSharedAnimStart(300.0f, 0.0f, 0.0f, self, (void *)"main_light.fab", 1, 1);
      GfxFabUpdate(g_uiSharedAnims[1]);
      GfxFabUpdate(g_uiSharedAnims[1]);
      UiTitlePlateInit(0, ((GfxSprite **)self->base.data)[0]);
      UiEquipStartEmblemZoomIn(self);
      UiEquipSetFrameFlags(self, true, 1);
      self->base.phaseStep = 1;
    }
    break;
  case 1:
    n = UiTitlePlateStep(0);
    n += UiEquipUpdateEmblemZoomIn(self);
    if (n == 2) {
      UiEquipStartEmblemHold(self);
      self->base.phaseStep = 2;
    }
    break;
  case 2:
    if (UiEquipUpdateEmblemHold(self)) {
      UiEquipStartPlayerIconsTween(self, 0);
      UiEquipPrepareEmblemTween(self, false);
      UiEquipStartPlayerLabelsTween(self, 0);
      UiEquipStartAllPlayerStamps(self, false);
      UiEquipStartPlayerDoneMarksTween(self, 0);
      UiEquipStartButtonGuideTween(self, 0);
      UiEquipResetIntroCounter(self);
      self->base.phaseStep = 3;
    }
    break;
  case 3: {
    s32 i;

    n = (u8)UiEquipUpdateEmblemTween(self, false);
    n += UiEquipUpdatePlayerIconsTween(self, 0);
    n += UiEquipUpdatePlayerLabelsTween(self, 0);
    n += UiEquipUpdatePlayerDoneMarksTween(self, 0);
    n += UiEquipUpdateButtonGuideTween(self, 0);
    if (n != 5) {
      break;
    }
    UiEquipStartGridPanelTween(self, 0);
    UiEquipStartRandomButtonTween(self, 0);
    UiEquipStartSprite516aTween(self, 0);
    UiEquipStartGridTween(self, 0);
    UiEquipStartPlayerTabTween(self, 0);
    for (i = 0; i < self->playerCount; i++) {
      UiEquipShowPlayerSlot(self, true, (u8)i);
      if (i != *(s32 *)self->localPlayer) {
        UiEquipShowPlayerGroup51e2(self, (u8)i, false);
      }
      UiEquipNetSetPeerHandicap(self, i, 100);
      ((GfxSprite **)self->base.data)[self->spriteIdx[10] + i]->flags &= ~1u;
    }
    UiEquipShowPlayerGroup51e2(self, (u8)*(s32 *)self->localPlayer, true);
    self->base.phaseStep = 4;
    break;
  }
  case 4:
    n = UiEquipUpdateGridPanelTween(self, 0);
    n += UiEquipUpdateRandomButtonTween(self, 0);
    n += UiEquipUpdateSprite516aTween(self, 0);
    n += UiEquipUpdateGridTween(self, 0);
    n += UiEquipUpdatePlayerTabTween(self, 0);
    if (n == 5) {
      UiEquipSetPendingPulse(self, true);
      UiEquipResetPlayerRecords(self, 0);
      self->onRandom = 0;
      self->editPlayer = (s8)*(s32 *)self->localPlayer;
      UiEquipStartPlayerStamp(self, 0, 0);
      UiEquipStartPlayerStamp(self, 1, 0);
      UiEquipResetCursor(self);
      self->base.phaseStep = 5;
      self->gearEditing = 0;
    }
    break;
  case 5: {
    s32 pick[2];
    s32 prev[2];
    s32 id;

    UiEquipUpdateCursorPulse(self);
    UiEquipPulseGridCursor(self);
    UiPulseStep(((GfxSprite **)self->base.data)[self->spriteCount],
                (UiPulse *)&self->tweens[self->spriteCount]);
    if (self->gearEditing == 1) {
      /* leaving the screen: close everything */
      if (SndHasManager()) {
        SndManagerPlay(SndGetManager(), 2, 0, 0);
      }
      UiEquipResetCursor(self);
      UiEquipHideGridCursors(self);
      UiEquipSetupSlotModels(self, true);
      UiEquipStartPlayerStamp(self, 0, 0);
      UiEquipStartPlayerStamp(self, 1, 0);
      self->base.phaseStep = 7;
      break;
    }
    if (self->base.pad->pressed & 0x2000) {
      /* Circle: ask whether to quit */
      self->base.phaseStep = 0x1a;
      if (!UiMsgWindowExists()) {
        UiMsgWindowEnsure();
      }
      ((UiMsgWindow *)UiMsgWindowGet())->mode = 0;
      UiMsgWindowOpen((UiMsgWindow *)UiMsgWindowGet(), 1, 0x1a);
      break;
    }
    if (self->peer[1 - *(s32 *)self->localPlayer].extra14 == 1) {
      /* the peer is leaving */
      self->base.phaseStep = 0x19;
      if (!UiMsgWindowExists()) {
        UiMsgWindowEnsure();
      }
      ((UiMsgWindow *)UiMsgWindowGet())->mode = 1;
      UiMsgWindowOpen((UiMsgWindow *)UiMsgWindowGet(), 0, 0x1e);
      if (NetPlayHasManager()) {
        NetPlayRequestAbort(NetPlayGetManager());
      }
      break;
    }
    r = UiEquipCheckGridConfirm(self);
    if (r != 0) {
      if (self->onRandom != 0 || self->peer[*(s32 *)self->localPlayer].bakugan != 0) {
        if (r == 1) {
          if (SndHasManager()) {
            SndManagerPlay(SndGetManager(), 0, 0, 0);
          }
          UiEquipResetCursor(self);
          UiEquipFlashGridSelection(self);
          self->gearEditing = 0;
          self->base.phaseStep = 0xb;
        } else {
          if (SndHasManager()) {
            SndManagerPlay(SndGetManager(), 3, 0, 0);
          }
        }
      }
      break;
    }
    pick[0] = self->peer[0].bakugan;
    pick[1] = self->peer[1].bakugan;
    prev[0] = pick[0];
    prev[1] = pick[1];
    if (UiEquipMoveGridCursor(self) != 0) {
      if (SndHasManager()) {
        SndManagerPlay(SndGetManager(), 1, 0, 0);
      }
      UiEquipResetCursor(self);
    }
    if (self->onRandom == 0) {
      pick[*(s32 *)self->localPlayer] = UiEquipMapBakuganIndex(self, 0, (u8)self->gridCursor);
      {
        SaveProfile *profile = SaveGetProfile();

        lp = *(s32 *)self->localPlayer;
        id = pick[lp];
        /* a Bakugan the player does not own is no pick */
        if ((u8)(profile->data->bakuganBitsA[id / 8] & (1 << (id % 8))) == 0) {
          pick[lp] = 0;
        }
      }
    } else {
      lp = *(s32 *)self->localPlayer;
      pick[lp] = 0;
    }
    if (UiEquipAdjustHandicap(self, (u8)lp) != 0) {
      if (SndHasManager()) {
        SndManagerPlay(SndGetManager(), 1, 0, 0);
      }
      lp = *(s32 *)self->localPlayer;
      UiEquipNetSetPeerHandicap(self, lp, self->handicap[lp]);
    }
    lp = *(s32 *)self->localPlayer;
    if (prev[lp] != pick[lp]) {
      UiEquipNetSetPeerBakugan(self, lp, pick[lp]);
    }
    break;
  }
  case 7:
    if (NetPlayHasManager() && self->gearEditing == 0) {
      u32 flags = NetPlayGetFlags(NetPlayGetManager()) | 0x1000000;

      NetPlaySetFlags(NetPlayGetManager(), flags);
    }
    UiEquipHideSelectionMarkers(self);
    UiEquipSetPendingPulse(self, false);
    UiEquipStartPlayerStamp(self, 0, 0);
    UiEquipStartPlayerStamp(self, 1, 0);
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
    n = UiTitlePlateStep(1);
    n += (u8)UiEquipUpdateEmblemTween(self, true);
    n += UiEquipUpdatePlayerIconsTween(self, 1);
    n += UiEquipUpdatePlayerLabelsTween(self, 1);
    n += UiEquipUpdateGridPanelTween(self, 1);
    n += UiEquipUpdateRandomButtonTween(self, 1);
    n += UiEquipUpdateSprite516aTween(self, 1);
    n += UiEquipUpdateGridTween(self, 1);
    n += UiEquipUpdatePlayerDoneMarksTween(self, 1);
    n += UiEquipUpdateFourPlayerDecorTween(self, 1);
    n += UiEquipUpdatePlayerTabTween(self, 1);
    n += UiEquipUpdateDoneSlotsTween(self, 1);
    n += UiEquipUpdateButtonGuideTween(self, 1);
    if (n == 13) {
      self->base.phaseStep++;
    }
    break;
  case 9:
    if (UiSharedAnimIsDone(self, (u32)-1, 1)) {
      GfxFader *fader = GfxGetActiveFader();

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
        UiEquipShowPlayerGroup51e2(self, (u8)self->editPlayer, false);
        UiEquipResetRandomPick(self);
        self->base.phaseStep = 0xc;
      }
    }
    break;
  case 0xc:
    if (UiEquipRandomPick(self) == 1) {
      if (SndHasManager()) {
        SndManagerPlay(SndGetManager(), 0, 0, 0);
      }
      UiEquipResetCursor(self);
      UiEquipFlashGridSelection(self);
      self->gearEditing = 0;
      n = UiEquipMapBakuganIndex(self, 0, (u8)self->gridCursor);
      UiEquipNetSetPeerBakugan(self, *(s32 *)self->localPlayer, n);
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
    UiEquipShowPlayerGroup51e2(self, (u8)self->editPlayer, false);
    UiEquipStartGearPanelTween(self, false, (u8)self->editPlayer);
    self->base.phaseStep++;
    break;
  case 0xf:
    if (UiEquipUpdateGearPanelTween(self, false, (u8)self->editPlayer)) {
      UiEquipResetGearCursor(self, (u8)self->editPlayer);
      self->base.phaseStep = 0x10;
    }
    break;
  case 0x10:
    UiEquipPulseGearCursor(self, (u8)self->editPlayer);
    UiEquipGlowOkButton(self, (u8)self->editPlayer);
    UiEquipUpdateGearCursorPulse(self, (u8)self->editPlayer);
    UiPulseStep(((GfxSprite **)self->base.data)[self->spriteCount],
                (UiPulse *)&self->tweens[self->spriteCount]);
    n = (u8)UiEquipNetCheckGearConfirm(self);
    if (n != 0) {
      if (n == 1) {
        if (SndHasManager()) {
          SndManagerPlay(SndGetManager(), 0, 0, 0);
        }
        UiEquipResetGearCursor(self, (u8)self->editPlayer);
        UiEquipFlashGearSelection(self);
        self->gearReturnToGrid = 0;
        self->base.phaseStep = 0x13;
      } else {
        if (SndHasManager()) {
          SndManagerPlay(SndGetManager(), 3, 0, 0);
        }
      }
    } else if (self->base.pad->pressed & 0x2000) {
      /* Circle: close the panel and go back to the grid */
      if (SndHasManager()) {
        SndManagerPlay(SndGetManager(), 2, 0, 0);
      }
      self->gearReturnToGrid = 1;
      UiEquipHideGearCursors(self, (u8)self->editPlayer);
      self->base.phaseStep = 0x11;
    } else if (self->peer[1 - *(s32 *)self->localPlayer].extra14 == 1) {
      self->base.phaseStep = 0x19;
      if (!UiMsgWindowExists()) {
        UiMsgWindowEnsure();
      }
      ((UiMsgWindow *)UiMsgWindowGet())->mode = 1;
      UiMsgWindowOpen((UiMsgWindow *)UiMsgWindowGet(), 0, 0x1b);
      if (NetPlayHasManager()) {
        NetPlayRequestAbort(NetPlayGetManager());
      }
    } else if (UiEquipNetMoveGearRow(self) == 1) {
      if (SndHasManager()) {
        SndManagerPlay(SndGetManager(), 1, 0, 0);
      }
      UiEquipResetGearCursor(self, (u8)self->editPlayer);
    } else if (UiEquipMoveGearCursor(self) == 1) {
      if (SndHasManager()) {
        SndManagerPlay(SndGetManager(), 1, 0, 0);
      }
      UiEquipResetGearCursor(self, (u8)self->editPlayer);
    } else {
      n = (u8)UiEquipCheckHelpButton(self);
      if (n != 0) {
        if (n == 1) {
          if (SndHasManager()) {
            SndManagerPlay(SndGetManager(), 0, 0, 0);
          }
          UiEquipResetHelpPopup(self);
          self->base.phaseStep = 0x16;
        } else {
          if (SndHasManager()) {
            SndManagerPlay(SndGetManager(), 3, 0, 0);
          }
        }
      }
    }
    break;
  case 0x11:
    UiEquipStartGearPanelTween(self, true, (u8)self->editPlayer);
    self->base.phaseStep++;
    break;
  case 0x12:
    if (UiEquipUpdateGearPanelTween(self, true, (u8)self->editPlayer)) {
      if (self->gearReturnToGrid != 0) {
        UiEquipResetCursor(self);
        UiEquipShowPlayerGroup51e2(self, (u8)self->editPlayer, true);
        self->base.phaseStep = 5;
      } else {
        self->base.phaseStep = 0x14;
      }
    }
    break;
  case 0x13:
    if (UiEquipWaitGearFlash(self) == 1) {
      if (self->gearRowFlags[*(s32 *)self->localPlayer] == 0) {
        UiEquipRefreshGearList(self, (u8)self->editPlayer);
        self->base.phaseStep = 0x10;
      } else {
        UiEquipHideGearCursors(self, (u8)self->editPlayer);
        self->base.phaseStep = 0x11;
      }
    }
    break;
  case 0x14: {
    s32 p = self->editPlayer;

    UiEquipNetSetPeerGear(self, p, self->gearPick[p][0], self->gearPick[p][1]);
    self->base.phaseStep++;
    break;
  }
  case 0x15:
    self->base.phaseStep = 0x18;
    break;
  case 0x16:
    if (UiEquipShowHelpPopup(self) == 1) {
      self->base.phaseStep = 0x10;
    }
    break;
  case 0x18:
    /* done: wait for the peer */
    if (self->peer[1 - *(s32 *)self->localPlayer].extra10 == 0x18) {
      UiEquipUpdatePlayerDoneMarkTween(self, 1, 0);
      UiEquipUpdatePlayerDoneMarkTween(self, 1, 1);
      self->base.phaseStep = 7;
    }
    if (self->gearEditing != 1 && self->base.phaseStep == 0x18 &&
        (self->base.pad->pressed & 0x2000)) {
      /* Circle: reopen the equipment panel */
      UiEquipHideGridCursors(self);
      self->base.phaseStep = 0xe;
    }
    if (self->peer[1 - *(s32 *)self->localPlayer].extra14 == 1) {
      self->base.phaseStep = 0x19;
      if (!UiMsgWindowExists()) {
        UiMsgWindowEnsure();
      }
      ((UiMsgWindow *)UiMsgWindowGet())->mode = 1;
      UiMsgWindowOpen((UiMsgWindow *)UiMsgWindowGet(), 0, 0x1b);
      if (NetPlayHasManager()) {
        NetPlayRequestAbort(NetPlayGetManager());
      }
    }
    break;
  case 0x19: {
    bool closed = true;

    if (UiMsgWindowExists()) {
      closed = false;
      if (UiMsgWindowIsClosed((UiMsgWindow *)UiMsgWindowGet())) {
        closed = true;
      }
    }
    if (closed) {
      self->gearEditing = 1;
      self->base.phaseStep = 7;
    }
    break;
  }
  case 0x1a: {
    /* quit question: choice 0 (or no window) leaves, any other answer returns to the grid */
    bool done = true;
    bool quit = true;

    if (UiMsgWindowExists()) {
      quit = false;
      if (UiMsgWindowIsClosed((UiMsgWindow *)UiMsgWindowGet())) {
        r = UiMsgWindowGetChoice((UiMsgWindow *)UiMsgWindowGet());
        if (r == 0) {
          quit = true;
        }
      } else {
        done = false;
        if (SaveHasProfile() && SaveProfileHasFlags(SaveGetProfile(), 0x80)) {
          UiMsgWindowRequestClose((UiMsgWindow *)UiMsgWindowGet());
        }
        if (self->peer[1 - *(s32 *)self->localPlayer].extra14 == 1) {
          UiMsgWindowRequestClose((UiMsgWindow *)UiMsgWindowGet());
          self->base.phaseStep = 0x19;
          if (!UiMsgWindowExists()) {
            UiMsgWindowEnsure();
          }
          ((UiMsgWindow *)UiMsgWindowGet())->mode = 1;
          UiMsgWindowOpen((UiMsgWindow *)UiMsgWindowGet(), 0, 0x1b);
          if (NetPlayHasManager()) {
            NetPlayRequestAbort(NetPlayGetManager());
          }
          break;
        }
      }
    }
    if (done) {
      if (quit) {
        self->base.phaseStep = 5;
        self->gearEditing = 1;
      } else {
        self->base.phaseStep = 5;
      }
    }
    break;
  }
  default: /* 6, 0x17 */
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
  switch (self->base.phaseStep) {
  case 5:
  case 0xb: case 0xc: case 0xd: case 0xe: case 0xf: case 0x10: case 0x11:
  case 0x12: case 0x13: case 0x14: case 0x15: case 0x16:
  case 0x18: case 0x19:
    UiEquipApplyPeerSelections(self);
    break;
  default:
    break;
  }
}
