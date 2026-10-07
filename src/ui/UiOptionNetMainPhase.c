// bdc 0x08972b0c UiOptionNetMainPhase
#include "bdc.h"

/* Ad-hoc variant of `UiOptionMainPhase` (entry 4 of the phase table `0x08a9d9f8`) of the
   battle-options screen (task 304, `maybe_UiScreen304Ctor`; `"option_battle_t_%02d"` /
   `"option_sol00"` sprites; edits the battle rules stored in profile words 0x18..0x1b): the host
   drives the same menu and the state is exchanged each frame through the
   `NetCharaPushMessage`/`NetCharaReadSlot` slots of player 0 (relayed into the world-map task
   0x136 with `UiWorldMapApplyNetState`); a lost connection (profile flag 0x80) aborts. */

void UiOptionNetMainPhase(UiOption *self)
{
  UiWorldMap *map;
  NetChara *chara;
  UiMsgWindow *win;
  SndManager *snd;
  int busy;
  int peer;
  int other;
  int ready;
  int changed;
  int moved;
  int next;
  u8 slid;
  u8 faded;
  s8 v1;
  s8 v2;
  s8 v3;
  u32 msg[10];

  map = (UiWorldMap *)CoreTaskFind(0x136);
  if (SaveGetProfileFlag0() != 0) {
    if (NetPlayHasManager()) {
      busy = 0;
      if (NetPlayHasManager()) {
        switch (self->base.phaseStep) {
        case 0:
        case 1:
        case 4:
        case 5:
        case 6:
        case 7:
        case 9:
          busy = 1;
          break;
        }
        if (!busy) {
          chara = NetCharaGetByIndex(0);
          if ((NetPlayGetFlags((NetPlay *)NetPlayGetManager()) & 0x2000000) != 0 &&
              NetCharaIsSyncHandshakeDone() != 0) {
            busy = 1;
          }
          if (!busy && chara != NULL) {
            /* Pull the peer's record, then our own echo. */
            if (NetPlayIsSynced((NetPlay *)NetPlayGetManager())) {
              peer = map->netSession == 0;
              if (NetCharaReadSlot(chara, peer, msg) && (msg[1] & 0x2000000) != 0) {
                UiWorldMapApplyNetState(&map->base, peer, (s16 *)&msg[2]);
              }
              memset(msg, 0, 0x28);
              if (NetCharaReadSlot(chara, map->netSession, msg)) {
                UiWorldMapApplyNetState(&map->base, map->netSession, (s16 *)&msg[2]);
              }
              map->player[map->netSession].cursorA = 0;
            }
            /* Publish our menu state through our player record. */
            memset(msg, 0, 0x28);
            msg[1] = 0x2000000;
            map->player[map->netSession].optCursor = (s8)self->cursor;
            map->player[map->netSession].setting[0] = self->values[1];
            map->player[map->netSession].setting[1] = self->values[2];
            map->player[map->netSession].setting[2] = self->values[3];
            map->player[map->netSession].unkE = (s16)self->base.phaseStep;
            map->player[map->netSession].slotTag = self->netDirty;
            map->player[map->netSession].optNetFlag = self->netFlag;
            memcpy(&msg[2], &map->player[map->netSession], 0x10);
            NetCharaPushMessage(chara, msg);
          }
        }
      }
    }
    if (SaveHasProfile() && SaveProfileHasFlags(SaveGetProfile(), 0x80) &&
        (self->base.phaseStep == 10 || self->base.phaseStep == 8 || self->base.phaseStep == 2)) {
      /* Connection lost: show message 0x1d and close. */
      if (!UiMsgWindowExists()) {
        UiMsgWindowEnsure();
      }
      win = (UiMsgWindow *)UiMsgWindowGet();
      win->mode = 1;
      UiMsgWindowOpen((UiMsgWindow *)UiMsgWindowGet(), 0, 0x1d);
      self->base.phaseStep = 4;
    }
  }

  switch ((u32)self->base.phaseStep) {
  case 0:
    UiOptionInitTitle(self);
    UiOptionInitHeader(self);
    UiOptionInitRowArrows(self);
    UiOptionInitRowPanels(self);
    UiOptionInitRowPlates(self);
    UiOptionInitRowNames(self);
    UiOptionInitRowFrames(self);
    UiOptionInitButtons(self);
    UiOptionInitButtonLabels(self);
    UiOptionRefreshValues(self);
    UiOptionRefreshAllRowArrows(self);
    UiOptionStartFade(self, 0);
    UiOptionStartSlide(self, 0);
    self->base.phaseStep = self->base.phaseStep + 1;
    break;
  case 1:
    slid = UiOptionUpdateSlide(self, 0);
    faded = UiOptionUpdateFade(self, 0);
    if ((u8)(slid + faded) == 2) {
      UiOptionResetCursor(self);
      UiOptionShowHelp(self, true);
      self->base.phaseStep = self->base.phaseStep + 1;
    }
    break;
  case 2:
    /* Host side: drive the menu. */
    UiOptionPulseCursor(self);
    UiOptionZoomButton(self);
    UiOptionBlinkButtonGlow(self);
    UiPulseStep(((GfxSprite **)self->base.data)[0x3a], &self->slots[0x3a].pulse);
    if (map->netSession != 0) {
      self->base.phaseStep = 8;
      break;
    }
    if (map->player[0].slotTag == 1 || map->player[1].slotTag == 1) {
      if (SndHasManager()) {
        snd = SndGetManager();
        SndManagerPlay(snd, 2, 0, 0);
      }
      self->netDirty = 1;
      self->base.phaseStep = 10;
      self->netFlag = 0;
    } else if (UiOptionCheckConfirm(self) == 1) {
      if (SndHasManager()) {
        snd = SndGetManager();
        SndManagerPlay(snd, 0, 0, 0);
      }
      UiOptionResetCursor(self);
      UiOptionStartButtonPress(self);
      self->base.phaseStep = 6;
    } else if ((self->base.pad->pressed & 0x2000) != 0) {
      if (SndHasManager()) {
        snd = SndGetManager();
        SndManagerPlay(snd, 2, 0, 0);
      }
      self->netDirty = 1;
      self->base.phaseStep = 10;
      self->netFlag = 0;
    } else if (UiOptionMoveCursorVertical(self) == 1) {
      if (SndHasManager()) {
        snd = SndGetManager();
        SndManagerPlay(snd, 1, 0, 0);
      }
      UiOptionResetCursor(self);
    } else if (UiOptionChangeValue(self) == 1) {
      if (SndHasManager()) {
        snd = SndGetManager();
        SndManagerPlay(snd, 1, 0, 0);
      }
      UiOptionStartArrowPress(self);
      UiOptionUpdateValueSprite(self);
      self->base.phaseStep = 3;
    } else if (UiOptionMoveCursorButtons(self) == 1) {
      if (SndHasManager()) {
        snd = SndGetManager();
        SndManagerPlay(snd, 1, 0, 0);
      }
      UiOptionResetCursor(self);
    }
    break;
  case 3:
    UiOptionPulseCursor(self);
    if (UiOptionAnimateValueChange(self) == 1) {
      UiOptionRefreshRowArrows(self, self->cursor);
      self->base.phaseStep = 2;
    }
    break;
  case 4:
    UiOptionShowHelp(self, false);
    UiOptionHideButtonGlow(self);
    UiOptionStartFade(self, 1);
    UiOptionStartSlide(self, 1);
    self->base.phaseStep = self->base.phaseStep + 1;
    break;
  case 5:
    slid = UiOptionUpdateSlide(self, 1);
    faded = UiOptionUpdateFade(self, 1);
    if ((u8)(slid + faded) == 2) {
      self->base.phaseStep = 7;
    }
    break;
  case 6:
    if (UiOptionWaitPress(self) == 1) {
      self->base.phaseStep = 10;
      self->netFlag = 0;
    }
    break;
  case 8:
    /* Guest side: mirror the host's record (player 0). */
    UiOptionPulseCursor(self);
    UiOptionZoomButton(self);
    UiOptionBlinkButtonGlow(self);
    UiPulseStep(((GfxSprite **)self->base.data)[0x3a], &self->slots[0x3a].pulse);
    if (map->netSession == 0) {
      break;
    }
    changed = 0;
    moved = 0;
    if (map->player[1].slotTag == 1) {
      if (map->player[0].slotTag == 1) {
        self->base.phaseStep = 10;
        self->netFlag = 0;
      }
      if ((self->base.pad->pressed & 0x2000) != 0) {
        if (SndHasManager()) {
          snd = SndGetManager();
          SndManagerPlay(snd, 2, 0, 0);
        }
        self->netDirty = 1;
        break;
      }
    }
    v1 = self->values[1];
    v2 = self->values[2];
    v3 = self->values[3];
    if ((s8)self->cursor != map->player[0].optCursor) {
      changed = 1;
      self->cursor = (u8)map->player[0].optCursor;
      moved = 1;
    }
    if (v1 != map->player[0].setting[0]) {
      self->arrowSide = !(map->player[0].setting[0] < v1);
      self->values[1] = map->player[0].setting[0];
      changed = 1;
    }
    if (v2 != map->player[0].setting[1]) {
      self->arrowSide = !(map->player[0].setting[1] < v2);
      self->values[2] = map->player[0].setting[1];
      changed = 1;
    }
    if (v3 != map->player[0].setting[2]) {
      self->arrowSide = !(map->player[0].setting[2] < v3);
      self->values[3] = map->player[0].setting[2];
      changed = 1;
    }
    if (changed) {
      if (SndHasManager()) {
        snd = SndGetManager();
        SndManagerPlay(snd, 1, 0, 0);
      }
      if (moved) {
        UiOptionResetCursor(self);
      } else {
        UiOptionStartArrowPress(self);
        UiOptionUpdateValueSprite(self);
        self->base.phaseStep = 3;
      }
    } else if (map->player[0].unkE == 10) {
      self->base.phaseStep = 10;
      self->netFlag = 0;
    }
    break;
  case 9:
    if (!UiMsgWindowExists() || UiMsgWindowIsClosed((UiMsgWindow *)UiMsgWindowGet())) {
      self->base.phaseStep = 4;
    }
    break;
  case 10:
    /* Sync step: wait until the peer reached it too, then leave or reload the defaults. */
    other = 1 - map->netSession;
    ready = map->player[other].unkE == -1;
    if ((self->netFlag & 1) == 0) {
      self->netFlag = self->netFlag | 1;
    } else if ((map->player[other].optNetFlag & 1) != 0) {
      ready = 1;
    }
    if (!ready) {
      break;
    }
    if (map->player[0].slotTag == 1 || map->player[1].slotTag == 1) {
      self->base.phaseStep = 4;
    } else if (map->player[0].optCursor == 4) {
      self->base.phaseStep = 4;
    } else {
      UiEquipResetBattleOptions();
      UiOptionSyncProfile(self, false);
      UiOptionRefreshValues(self);
      UiOptionRefreshAllRowArrows(self);
      next = 8;
      if (map->netSession == 0) {
        next = 2;
      }
      self->base.phaseStep = next;
    }
    break;
  default:
    UiOptionApply(self);
    self->base.phase = 3;
    self->base.phaseStep = 0;
    break;
  }
}
