// bdc 0x088532c8 BtlMainPhaseFinish
#include "bdc.h"

/* Phase 2 of the battle main task: the end of a battle, a step machine on `phaseStep`.
   -1: stops BGM, loop sounds and camera shake, locks control and (rule mode 2, once per battle)
       bumps the per-team win counters; 0: time-up battles (outcome 3) go to the judge step 5,
       others to 1; 1: waits for the finish cinematic (task 0x14a), the intro demo and the result
       jingle (BGM player 1); 5: judges the outcome, records the round and starts the result demo
       and the HUD battle end; 6: waits for result window 8; 10/11: lets the demo run, then stops
       BGM and voices; 20: decides whether the battle restarts in place (`rematch`), syncs net play,
       records wins/losses/battles; 21: fades the scene to black; 100: black clear colour, mutes the
       listener; 101: releases the stream files, voice package, emitters and the sound groups, then
       records a won story stage and removes the HUD tasks (0x6e, 0x6c); 102: waits for task 0x2756;
       103: removes itself. Profile flags 0x4880 / 0x80 / 0x400 detour through the message-window
       steps 200, 201, 210, 211; a rematch ends in step 999, which tears the battle down for a
       fresh start (BtlMainTeardown). */
void BtlMainPhaseFinish(BtlMain *self)
{
  switch (self->phaseStep) {
  case -1: {
    s32 team;

    BtlMainUpdateScene(self);
    self->word53c = 50;
    self->dimColor[0] = 0.0f;
    self->dimColor[1] = 0.0f;
    self->dimColor[2] = 0.0f;
    self->dimColor[3] = 0.0f;
    BtlStopBgm();
    BtlStopAllUnitLoopSounds(self, 1);
    GfxCameraStopShake(g_gfxActiveCamera);
    BtlSetControlLockAll(1);
    if (self->winsCounted == 0 && g_scriptGlobalVars[8] == 2) {
      for (team = 0; team < 4; team++) {
        if (BtlMainGetTeamOutcome(self, team) == 1) {
          SaveProfileAddWord(SaveGetProfile(), team + 0x27, 1);
        }
      }
      self->winsCounted = 1;
    }
    self->phaseStep = 0;
    break;
  }

  case 0:
    self->phaseStep = (g_btlBattleOutcome == 3) ? 5 : 1;
    break;

  case 1:
    BtlMainUpdateScene(self);
    if (SaveGetProfileFlag0() != 0 && SaveHasProfile() &&
        SaveProfileHasFlags(SaveGetProfile(), 0x4880)) {
      self->phaseStep = 200;
      break;
    }
    if (CoreTaskExists(0x14a) != 0) {
      break;
    }
    if (!BtlDemoIsFinished()) {
      break;
    }
    if (SndBgmPlayerExists(1) && SndBgmPlayerIsStopped(SndBgmPlayerGet(1)) == 0) {
      break;
    }
    self->phaseStep = (self->resultDemo != -1) ? 10 : 20;
    break;

  case 5:
    g_btlBattleOutcome = BtlMainJudgeOutcome(self, 0);
    if (BtlMainIsDraw(self)) {
      BtlMainRecordRoundResult(self, 2);
      g_btlBattleOutcome = 4;
    } else if (BtlMainIsBehindOnTimeUp(self, 0)) {
      g_btlBattleOutcome = 2;
    } else {
      BtlMainRecordRoundResult(self, 1);
      g_btlBattleOutcome = 1;
    }
    if (BtlMainIsMatchDrawn(self)) {
      g_btlBattleOutcome = 4;
    } else if (BtlMainIsMatchWon(self)) {
      g_btlBattleOutcome = 1;
    } else if (BtlMainIsMatchLost(self)) {
      g_btlBattleOutcome = 2;
    }
    self->resultDemo = BtlMainStartResultDemo(self, g_btlBattleOutcome);
    BtlHudStartBattleEnd(UiGetTalkTask());
    self->phaseStep++;
    /* fall through */
  case 6:
    BtlMainUpdateScene(self);
    (void)UiGetTalkTask();
    if (UiGetWindowActive(8) != 0) {
      self->phaseStep = (self->resultDemo != -1) ? 10 : 20;
    }
    break;

  case 10:
    BtlResetUnitWord15c();
    g_btlDemoReady = 1;
    UiSetWindowActive(2, 1);
    self->phaseStep++;
    /* fall through */
  case 11:
    if (SaveGetProfileFlag0() != 0) {
      BtlMainUpdateQuitPrompt(self);
    }
    if (CoreTaskExists(0x65) != 0) {
      break;
    }
    SndBgmCancelChannel(0);
    SndBgmQueueStop(0.5f, 0);
    SndBgmCancelChannel(1);
    SndBgmQueueStop(0.5f, 1);
    SndManagerFadeOutAllVoices(SndGetManager());
    self->dimColor[3] = 1.0f;
    self->phaseStep = 20;
    if (NetPlayHasManager()) {
      NetPlaySetFlags(NetPlayGetManager(), 0x9000000);
    }
    break;

  case 20: {
    if (SaveGetProfileFlag0() != 0 && SaveHasProfile() &&
        SaveProfileHasFlags(SaveGetProfile(), 0x4880)) {
      self->phaseStep = 200;
      break;
    }
    if (g_scriptGlobalVars[8] == 2 && (g_scriptGlobalVars[3] == 2 || g_scriptGlobalVars[3] == 9)) {
      self->rematch = 1;
      self->keepRounds = 0;
    }
    if (g_btlBattleOutcome == 2 && g_scriptGlobalVars[8] == 1 && BtlGetExitScene(self) == 2) {
      self->rematch = 1;
      self->keepRounds = 0;
    }
    if (BtlMainIsMatchUndecided(self)) {
      self->rematch = 1;
      self->keepRounds = 1;
    } else if (SaveGetProfileFlag0() != 0 && NetCharaMgrIsActive() != 0) {
      if (NetCharaIsSyncHandshakeDone() == 0) {
        NetChara *chara;
        u32 record[10];

        if (NetSyncStateIs2() == 0) {
          break;
        }
        chara = NetCharaGetByIndex(0);
        if (chara == NULL) {
          break;
        }
        if (NetCharaGetReadyFrames(chara) < 6) {
          break;
        }
        NetCharaReadSlot(chara, 0, record);
        break;
      }
      BtlNetSetBattleFlags(self, 0);
    }
    BtlResetUnitWord15c();
    if (SaveGetProfileFlag0() == 0 && self->keepRounds == 0 && g_scriptGlobalVars[8] == 2) {
      bool counted = 1;
      s32 kind = 0;
      BtlBakugan *player = BtlGetPlayerBakugan();
      SaveProfile *records;
      SaveProfile *profile;

      if (player != NULL) {
        kind = (s32)player->base.base.unk08;
      }
      switch (g_btlBattleOutcome) {
      case 1:
        records = SaveGetProfile();
        profile = SaveGetProfile();
        SaveRecordAddWins(records, kind, (s32)profile->words[0x4a], 1);
        break;
      case 2:
      case 3:
        records = SaveGetProfile();
        profile = SaveGetProfile();
        SaveRecordAddLosses(records, kind, (s32)profile->words[0x4a], 1);
        break;
      case 4:
        break;
      default:
        counted = 0;
        break;
      }
      if (counted) {
        records = SaveGetProfile();
        profile = SaveGetProfile();
        SaveRecordAddBattles(records, kind, (s32)profile->words[0x4a], 1);
      }
    }
    SndManagerFadeOutAllVoices(SndGetManager());
    self->phaseStep++;
  }
    /* fall through */
  case 21: {
    float frames = 30.0f;
    float alpha;

    if (BtlMainIsMatchUndecided(self)) {
      frames = 10.0f;
    }
    alpha = self->dimColor[3] + 1.0f / frames;
    self->dimColor[3] = alpha;
    if (alpha < 1.0f) {
      break;
    }
    self->dimColor[3] = 1.0f;
    self->phaseStep = 100;
  }
    /* fall through */
  case 100: {
    GfxDisplay *display = g_gfxDisplay;

    /* clear colour = black (lv.q/sv.q quad copy) */
    display->clearColor[0] = g_colorBlack.x;
    display->clearColor[1] = g_colorBlack.y;
    display->clearColor[2] = g_colorBlack.z;
    display->clearColor[3] = g_colorBlack.w;
    BtlStopAllUnitLoopSounds(self, 1);
    if (SndHasListener()) {
      SndListenerSetMuted(SndGetListener(), 1);
    }
    SndManagerFadeOutAllVoices(SndGetManager());
    self->phaseStep++;
  }
    /* fall through */
  case 101: {
    s32 i;
    bool pending;

    if (CoreTaskExists(0x2726) != 0) {
      break;
    }
    if (self->rematch == 0) {
      SndStreamFileRelease(10);
      if (SndStreamFileIsLoaded(10)) {
        break;
      }
      if (!SndStreamFileRelease(-1)) {
        break;
      }
    }
    if (SndIsGroupLoaderIdle() == 0) {
      break;
    }
    if (SaveGetProfileFlag0() != 0) {
      if (SaveHasProfile() && SaveProfileHasFlags(SaveGetProfile(), 0x80)) {
        g_scriptGlobalVars[3] = 6;
        g_btlBattleOutcome = 6;
        self->rematch = 0;
        self->keepRounds = 0;
        self->phaseStep = 210;
        break;
      }
      if (SaveHasProfile() && SaveProfileHasFlags(SaveGetProfile(), 0x400) &&
          !SaveProfileHasFlags(SaveGetProfile(), 0x2000)) {
        g_scriptGlobalVars[3] = 10;
        g_btlBattleOutcome = 7;
        self->rematch = 0;
        self->keepRounds = 0;
        self->phaseStep = 210;
        break;
      }
    }
    if (SndBgmPlayerGetTrackId(SndBgmPlayerGet(0)) != -1) {
      break;
    }
    if (SndBgmPlayerGetTrackId(SndBgmPlayerGet(1)) != -1) {
      break;
    }
    if (self->rematch != 0) {
      self->phaseStep = 999;
      break;
    }
    if (!SndVoicePacIsIdle()) {
      SndVoicePacRelease();
      break;
    }
    if (SndHasListener()) {
      SndEmitterDestroyAll(SndGetListener());
    }
    pending = 0;
    for (i = 0; i < 56; i++) {
      if (!SndManagerUnloadGroup(SndGetManager(), g_btlSoundGroupIds[i])) {
        SndManagerFadeOutAllVoices(SndGetManager());
        pending = 1;
        break;
      }
    }
    if (pending) {
      break;
    }
    BtlStageSuspendEventScript();
    g_scriptGlobalVars[3] = BtlGetExitScene(self);
    if (g_btlBattleOutcome == 1) {
      ActorStageObjRecordWalkNop();
      if (g_scriptGlobalVars[8] == 1) {
        BtlMainRecordStageClear(self);
        g_scriptGlobalVars[14] = BtlGetStoryWinStageValue(self);
        if (UiTalkTaskExists() != 0) {
          u8 stage = (u8)g_scriptGlobalVars[1];
          u8 id = (u8)SaveProfileGetWord(SaveGetProfile(), 3);
          u8 rank = (u8)BtlResultGetOverallRank(UiGetTalkTask());
          u16 score = (u16)BtlResultGetBattleScoreItem(UiGetTalkTask());

          SaveProfileRecordStageRank(stage, id, rank, score);
        }
      }
    }
    CoreTaskRemoveById(0x6e);
    CoreTaskRemoveById(0x6c);
    self->phaseStep++;
    break;
  }

  case 102:
    if (CoreTaskExists(0x2756) == 0) {
      self->phaseStep++;
    }
    break;

  case 103:
    CoreTaskRemove(&self->base, 1);
    break;

  case 200: {
    UiMsgWindow *win;

    if (UiGetWindowActive(9) != 0 && UiGetWindowActive(10) == 0) {
      break;
    }
    if (CoreTaskFind(0x14a) != NULL) {
      CoreTaskRemoveById(0x14a);
      break;
    }
    if (CoreTaskFind(0x14b) != NULL) {
      CoreTaskRemoveById(0x14b);
      break;
    }
    if (!UiMsgWindowExists()) {
      UiMsgWindowEnsure();
    }
    win = UiMsgWindowGet();
    win->depth = GfxGetActiveFader()->sortKey + 1.0f;
    if (SaveProfileHasFlags(SaveGetProfile(), 0x80)) {
      win = UiMsgWindowGet();
      win->mode = 1;
      UiMsgWindowOpen(UiMsgWindowGet(), 0, 0x1d);
      if (SaveHasProfile()) {
        SaveProfileClearFlags(SaveGetProfile(), 0x80);
      }
      self->phaseStep = 201;
    } else {
      self->phaseStep = 101;
    }
    break;
  }

  case 201: {
    CoreTask *demo;
    bool closed = 1;

    if (!BtlDemoIsFinished()) {
      break;
    }
    demo = CoreTaskFind(0x65);
    if (demo != NULL) {
      BtlDemoFinish(demo);
    }
    if (UiMsgWindowExists()) {
      closed = 0;
      if (UiMsgWindowIsClosed(UiMsgWindowGet())) {
        closed = 1;
      }
    }
    if (!closed) {
      break;
    }
    g_btlBattleOutcome = 6;
    self->phaseStep = 101;
    self->rematch = 0;
    break;
  }

  case 210: {
    UiMsgWindow *win;

    self->phaseStep = 211;
    if (!UiMsgWindowExists()) {
      UiMsgWindowEnsure();
    }
    win = UiMsgWindowGet();
    win->mode = 1;
    if (SaveProfileHasFlags(SaveGetProfile(), 0x80)) {
      UiMsgWindowOpen(UiMsgWindowGet(), 0, 0x1d);
      if (SaveHasProfile()) {
        SaveProfileClearFlags(SaveGetProfile(), 0x80);
      }
    } else {
      UiMsgWindowOpen(UiMsgWindowGet(), 0, 0x17);
      if (SaveHasProfile()) {
        SaveProfileClearFlags(SaveGetProfile(), 0x400);
      }
    }
    break;
  }

  case 211: {
    bool closed = 1;

    if (UiMsgWindowExists()) {
      closed = 0;
      if (UiMsgWindowIsClosed(UiMsgWindowGet())) {
        closed = 1;
      }
    }
    if (!closed) {
      break;
    }
    g_btlBattleOutcome = 6;
    self->phaseStep = 101;
    self->rematch = 0;
    break;
  }

  case 999:
    if (CoreTaskExists(0x65) != 0) {
      if (!BtlDemoIsFinished()) {
        break;
      }
      CoreTaskRemoveById(0x65);
    }
    SndManagerFadeOutAllVoices(SndGetManager());
    BtlMainTeardown(self);
    break;

  default:
    break;
  }
}
