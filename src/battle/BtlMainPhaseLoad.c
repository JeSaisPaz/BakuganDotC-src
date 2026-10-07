// bdc 0x08851908 BtlMainPhaseLoad
#include "bdc.h"

/* Phase 0 of the battle main task (table `0x08a66444`): the load/setup state machine on `phaseStep`,
   one step (or a run of fall-through steps) per frame.
   0: makes sure the fader and the text renderer exist, opens the now-loading screen, fades in from
      black over 20 frames, sets the stage from the event id (`BtlStageSetFromEventId`), remaps
      stages 0x24/0x25 to 0x26/0x27 in rule mode 2, applies the story battle 0x18 tweaks, fills
      `g_btlUnitKinds` from profile words 3..6 / script globals 4..7 / `g_btlStoryUnitKinds`,
      starts the Bakugan texture loader task 0x1e3 when no loader entry exists and a kind repeats,
      clears `roundResults`.
   1-14: loads the arena package, preloads stream 10 (except story battle 0x18), loads the stage voice
      package, `BATTLE_SE.pac` and its sound groups 0x41, 0x24 and 2, then the language's
      `battle_indicator.lzs`.
   15: once the fade is over and `field8` is 0, builds the scene (unit list, effect sets, stage map,
      attack/item systems, match state) and either jumps to 19 (story mode), spawns the units again
      (16, rematch) or creates a load request per team kind (17).
   16-19: spawns the team in slot order (`BtlMainSpawnBakugan`) and aims the camera at the player.
   20-23: loads the units' voice banks (plus group 0x4f/0x4e on story stages 8/9), finishes the camera
      setup and, once the loading screen reports state 3 and the fader is done, fades to black.
   24-26: network battles: arms the battle flags and waits (at most 90 frames before showing the
      status message) for the peers to sync, then a 30-frame countdown.
   27-31: closes the loading screen, fades in, updates the scene, starts the appear demo unless it is
      skipped (rematch, `skipAppearDemo`, or network profile flags 0x4880), preloads and then plays the
      BGM (not when profile flag 0x80 is set in a network battle), sets the clear colour and enables
      item spawning outside network battles.
   Any step >= 32 (or negative): clears the player's queued actions to 0x40 outside NetPlay, clears the
   rematch/round flags, resets `phaseStep`, stores `ActorStageObjCountStandingTargets` and moves the
   task (and its draw phase) to phase 9 (story battle) or 1. Returns nothing. */

void BtlMainPhaseLoad(BtlMain *self)

{
  GfxFader *fader;
  GfxDisplay *display;
  float *clearColor;
  float stageF;
  IoLzsPackage *package;
  const char *path;
  char *name;
  const VtblEntry *entry;
  UiLoading *loading;
  BtlBakugan *unit;
  CoreObjectList *units;
  CoreObject *obj;
  NetChara *chara;
  BtlBakuganTexLoaderTask *texLoader;
  s32 bank;
  s32 stage;
  s32 index;
  s32 state;
  s32 phase;
  u32 used;
  s32 i;
  bool ok;

  switch (self->phaseStep) {
  case 0:
    if (!GfxFaderIsReady()) {
      GfxFaderSlotsInit(NULL);
    }
    if (!UiTextRenderExists()) {
      UiTextRenderEnsure();
      ((UiTextBox *)UiTextRenderGetBox())->packetDepth = 1500.0f;
    }
    if (!UiLoadingIsOpen()) {
      UiLoadingOpen();
    }
    GfxGetActiveFader()->sortKey = 1800.0f;
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
    GfxFaderStart(GfxGetActiveFader(), 20);
    BtlStageSetFromEventId();
    if (g_scriptGlobalVars[8] == 2) {
      if (g_scriptGlobalVars[1] == 0x24) {
        g_scriptGlobalVars[1] = 0x26;
      }
      if (g_scriptGlobalVars[1] == 0x25) {
        g_scriptGlobalVars[1] = 0x27;
      }
    }
    if (g_scriptGlobalVars[8] == 1 && g_scriptGlobalVars[1] == 0x18) {
      if (!CoreBitsetTest(0x20, g_scriptGlobalBits)) {
        self->skipAppearDemo = 1;
      }
      if (SaveGetProfile()->data->playthrough == 0) {
        SaveProfileSetWord(SaveGetProfile(), 3, 2);
      }
    }
    g_btlUnitKinds[0] = SaveProfileGetWord(SaveGetProfile(), 3);
    g_btlUnitKinds[1] = SaveProfileGetWord(SaveGetProfile(), 4);
    g_btlUnitKinds[2] = SaveProfileGetWord(SaveGetProfile(), 5);
    g_btlUnitKinds[3] = SaveProfileGetWord(SaveGetProfile(), 6);
    g_btlUnitKinds[4] = 0;
    if (CoreBitsetTest(0, g_scriptGlobalBits)) {
      g_btlUnitKinds[0] = g_scriptGlobalVars[4];
      SaveProfileSetWord(SaveGetProfile(), 3, g_btlUnitKinds[0]);
    }
    if (g_btlUnitKinds[0] == 0) {
      g_btlUnitKinds[0] = g_scriptGlobalVars[4];
      g_btlUnitKinds[1] = g_scriptGlobalVars[5];
      g_btlUnitKinds[2] = g_scriptGlobalVars[6];
      g_btlUnitKinds[3] = g_scriptGlobalVars[7];
      SaveProfileSetWord(SaveGetProfile(), 3, g_btlUnitKinds[0]);
      SaveProfileSetWord(SaveGetProfile(), 4, g_btlUnitKinds[1]);
      SaveProfileSetWord(SaveGetProfile(), 5, g_btlUnitKinds[2]);
      SaveProfileSetWord(SaveGetProfile(), 6, g_btlUnitKinds[3]);
      used = 0;
      for (i = 0; i < 4; i++) {
        if (g_btlUnitKinds[i] != -1) {
          used++;
        }
      }
      SaveProfileSetWord(SaveGetProfile(), 0x16, used);
    }
    if (g_scriptGlobalVars[8] == 1) {
      stageF = (float)g_scriptGlobalVars[1];
      if (stageF < 0.0f) {
        index = 0;
      } else if (stageF <= 39.0f) {
        index = (s32)stageF;
      } else {
        index = 39;
      }
      g_btlUnitKinds[1] = g_btlStoryUnitKinds[index][0];
      g_btlUnitKinds[2] = g_btlStoryUnitKinds[index][1];
      g_btlUnitKinds[3] = g_btlStoryUnitKinds[index][2];
      SaveProfileSetWord(SaveGetProfile(), 4, g_btlUnitKinds[1]);
      SaveProfileSetWord(SaveGetProfile(), 5, g_btlUnitKinds[2]);
      SaveProfileSetWord(SaveGetProfile(), 6, g_btlUnitKinds[3]);
      if (index == 10) {
        BtlAiDisableTargetLossHook();
      }
    }
    if (!(g_btlTexLoaderCount > 0) && BtlKindsHaveDuplicate(g_btlUnitKinds) != 0) {
      texLoader = (BtlBakuganTexLoaderTask *)CoreTaskCreate(0x1e3, 100);
      BtlBakuganTexLoaderTaskRequest(texLoader, g_btlUnitKinds);
    }
    for (i = 0; i < 5; i++) {
      self->roundResults[i] = 0;
    }
    self->phaseStep++;
    /* fallthrough */
  case 1:
    if (CoreTaskExists(0x1e3)) {
      break;
    }
    package = (IoLzsPackage *)self->packages[0];
    path = BtlStageGetArenaDataB(g_scriptGlobalVars[1]);
    if (IoLzsPackageStartLoad(package, path, 10, 1, 0) == 0) {
      break;
    }
    g_gfxTexVramEnabled = 1;
    if (IoLzsPackagePoll((IoLzsPackage *)self->packages[0], 1) == 0) {
      break;
    }
    ok = true;
    if (g_scriptGlobalVars[8] == 1 && g_scriptGlobalVars[1] == 0x18) {
      ok = false;
    }
    if (ok) {
      SndStreamFilePreload(10);
    }
    self->phaseStep++;
    break;
  case 2:
    ok = true;
    if (g_scriptGlobalVars[8] == 1 && g_scriptGlobalVars[1] == 0x18) {
      ok = false;
    }
    if (ok && !SndStreamFileIsLoaded(10)) {
      break;
    }
    self->phaseStep++;
    /* fallthrough */
  case 3:
    self->phaseStep++;
    /* fallthrough */
  case 4:
    if (!SndVoicePacLoad(g_scriptGlobalVars[1] + 1)) {
      if (!SndVoicePacIsIdle()) {
        SndVoicePacRelease();
      }
      break;
    }
    self->phaseStep++;
    /* fallthrough */
  case 5:
    if (!SndVoicePacIsLoaded()) {
      break;
    }
    self->phaseStep++;
    /* fallthrough */
  case 6:
    if (IoLzsPackageStartLoad((IoLzsPackage *)self->packages[2], "sound/SE_pac/BATTLE_SE.pac", 2, 1, 0) == 0) {
      break;
    }
    if (IoLzsPackagePoll((IoLzsPackage *)self->packages[2], 1) == 0) {
      break;
    }
    self->phaseStep++;
    break;
  case 7:
    if (BtlLoadSoundGroupFromPac(self, self->packages[2], 0x41) == 0) {
      break;
    }
    self->phaseStep++;
    /* fallthrough */
  case 8:
    if (!SndManagerLoadGroup(SndGetManager(), 0x41)) {
      break;
    }
    self->phaseStep++;
    /* fallthrough */
  case 9:
    if (BtlLoadSoundGroupFromPac(self, self->packages[2], 0x24) == 0) {
      break;
    }
    self->phaseStep++;
    /* fallthrough */
  case 10:
    if (!SndManagerLoadGroup(SndGetManager(), 0x24)) {
      break;
    }
    self->phaseStep++;
    /* fallthrough */
  case 11:
    if (BtlLoadSoundGroupFromPac(self, self->packages[2], 2) == 0) {
      break;
    }
    self->phaseStep++;
    /* fallthrough */
  case 12:
    if (!SndManagerLoadGroup(SndGetManager(), 2)) {
      break;
    }
    self->phaseStep++;
    /* fallthrough */
  case 13:
    name = self->stagePackName;
    path = SaveGetLanguageDirName();
    sprintf(name, "data/2d/%s/battle_indicator.lzs", path);
    self->phaseStep++;
    /* fallthrough */
  case 14:
    if (IoLzsPackageStartLoad((IoLzsPackage *)self->packages[1], self->stagePackName, 10, 1, 0) == 0) {
      break;
    }
    if (IoLzsPackagePoll((IoLzsPackage *)self->packages[1], 1) == 0) {
      break;
    }
    self->phaseStep++;
    break;
  case 15:
    if (!GfxFaderIsFinished(GfxGetActiveFader())) {
      break;
    }
    if (self->field8 != 0) {
      break;
    }
    CoreBitsetClear(0x23, g_scriptGlobalBits);
    self->fieldFlags[0] = 1;
    g_gfxTexVramEnabled = 0;
    BtlInitBakuganList(&self->modelLists[0]);
    UiHpGaugeGroupReset();
    ActorBallSetList(&self->modelLists[2]);
    ActorStageObjSystemInit();
    BtlMainCreateStageEffectSet(self, CorePackChainFind(g_ioLzsPackages, "particle_00.ptb"));
    BtlMainCreateUnitEffectSet(self, CorePackChainFind(g_ioLzsPackages, "particle_01.ptb"));
    GfxPuffLoadTexture();
    GfxMeshObjClearLists();
    BtlSetAnimPhaseCounter(0x1b);
    BtlStageLoadMap(g_scriptGlobalVars[1], &self->modelLists[1]);
    BtlSetAnimPhaseCounter(0);
    BtlAttackSystemInit(self->stageEffects);
    BtlItemSystemInit(self->stageEffects, &self->modelLists[1]);
    BtlMainResetMatchState(self);
    if (BtlIsStoryBattle()) {
      self->busy = 1;
    }
    self->spawnIndex = 0;
    if (SaveGetProfileFlag0()) {
      CoreRandResetSeed();
      CoreRandLoadVfpuState();
    }
    BtlLoadRequestListEnsure();
    if (g_scriptGlobalVars[8] == 1) {
      self->phaseStep = 0x13;
    } else if (self->rematch != 0) {
      self->phaseStep = 0x10;
    } else {
      for (i = 0; i < 4; i++) {
        if (g_btlUnitKinds[i] != -1) {
          BtlLoadRequestCreate(g_btlUnitKinds[i]);
        }
      }
      self->phaseStep = 0x11;
    }
    break;
  case 0x10:
    do {
      BtlMainSpawnBakugan(self, &self->spawnIndex, g_btlUnitKinds[self->spawnIndex]);
    } while (self->spawnIndex < 4);
    if (!(self->spawnIndex < 4)) {
      self->phaseStep = 0x16;
    }
    break;
  case 0x11:
    if (!BtlLoadRequestsUpdate()) {
      break;
    }
    self->phaseStep = 0x12;
    /* fallthrough */
  case 0x12:
    do {
      BtlMainSpawnBakugan(self, &self->spawnIndex, g_btlUnitKinds[self->spawnIndex]);
    } while (self->spawnIndex < 4);
    if (!(self->spawnIndex < 4)) {
      self->phaseStep = 0x14;
    }
    break;
  case 0x13:
    if (!BtlLoadRequestsUpdate()) {
      break;
    }
    self->fieldFlags[1] = 1;
    if (self->fieldFlags[2] == 0) {
      break;
    }
    unit = BtlGetPlayerBakugan();
    if (unit == NULL) {
      break;
    }
    BtlCameraSetTarget(&self->camera, unit);
    if (unit->base.base.unk08 == 10) {
      self->camera.basePitch = 0.34906584f;
      self->camera.lockOnPitch = 0.34906584f;
    }
    BtlCameraSetDefaultFollow(&self->camera, 1);
    g_gfxActiveCamera = &self->camera.base;
    self->phaseStep = 0x14;
    break;
  case 0x14:
    ok = true;
    if (SndHasManager()) {
      units = BtlGetBakuganList();
      if (units != NULL) {
        for (obj = units->head; obj != NULL; obj = obj->next) {
          bank = ((BtlBakugan *)obj)->voiceBank;
          if (!SndManagerLoadGroup(SndGetManager(), bank)) {
            ok = false;
            break;
          }
        }
      }
      if (g_scriptGlobalVars[8] == 1) {
        stage = g_scriptGlobalVars[1];
        if (stage == 8) {
          if (!SndManagerLoadGroup(SndGetManager(), 0x4f)) {
            ok = false;
          }
        } else if (stage == 9) {
          if (!SndManagerLoadGroup(SndGetManager(), 0x4e)) {
            ok = false;
          }
        }
      }
    }
    if (ok) {
      self->phaseStep++;
    }
    break;
  case 0x15:
    if (SndHasManager() && SndManagerUpdateGroupSlots(SndGetManager())) {
      self->phaseStep++;
    }
    break;
  case 0x16:
    if (self->camera.target == NULL) {
      printf("WARNING !!!\n");
      printf("CTBattleTask::  m_BattleCamera.GetOwner() == NULL !!\n");
      unit = BtlGetPlayerBakugan();
      BtlCameraSetTarget(&self->camera, unit);
      if (unit->base.base.unk08 == 10) {
        self->camera.basePitch = 0.34906584f;
        self->camera.lockOnPitch = 0.34906584f;
      }
      BtlCameraSetDefaultFollow(&self->camera, 1);
      g_gfxActiveCamera = &self->camera.base;
    }
    GfxCameraStopShake(&self->camera.base);
    BtlCameraUpdate(&self->camera);
    ActorStageObjUpdateAll();
    BtlCameraBindListener(&self->camera);
    UiSetWindowActive(1, 1);
    BtlSetControlLockAll(1);
    self->phaseStep++;
    break;
  case 0x17:
    loading = CoreTaskFind(0x2774);
    if (loading == NULL) {
      self->phaseStep++;
      break;
    }
    /* UiLoadingGetField(loading, 3): the loading screen's state */
    entry = &((const VtblEntry *)loading->base.vtable)[6];
    state = ((s32 (*)(void *, u32))entry->fn)((char *)loading + entry->delta, 3);
    if (GfxFaderIsFinished(GfxGetActiveFader()) && state == 3) {
      GfxGetActiveFader()->sortKey = 1800.0f;
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
      GfxFaderStart(GfxGetActiveFader(), 20);
      self->phaseStep++;
    }
    break;
  case 0x18:
    if (self->rematch == 0 && SaveGetProfileFlag0()) {
      BtlNetSetBattleFlags(self, true);
      NetCharaResetAllSync();
      NetModeFlagSet();
      self->netWaitFrames = 0;
    }
    self->phaseStep++;
    break;
  case 0x19:
    if (self->rematch != 0) {
      self->phaseStep++;
      break;
    }
    if (!SaveGetProfileFlag0()) {
      self->netWaitFrames = 0;
      self->phaseStep++;
      break;
    }
    if (!NetPlayHasManager()) {
      self->phaseStep++;
      break;
    }
    if (NetPlayIsSynced(NetPlayGetManager())) {
      NetPlaySetFlags(NetPlayGetManager(), 0x9000000);
      NetStatusSetMessage(0, 0);
      self->netWaitFrames = 30;
      self->phaseStep++;
    } else if (self->netWaitFrames < 91) {
      self->netWaitFrames++;
    } else {
      NetStatusSetMessage(1, 0);
    }
    chara = NetCharaGetByIndex(0);
    if (chara != NULL) {
      NetCharaSetReady(chara);
    }
    break;
  case 0x1a:
    if (self->rematch != 0) {
      self->phaseStep++;
      break;
    }
    if (!SaveGetProfileFlag0()) {
      self->phaseStep++;
      break;
    }
    if (!NetPlayHasManager()) {
      self->phaseStep++;
      break;
    }
    if (self->netWaitFrames > 0) {
      self->netWaitFrames--;
    } else {
      self->phaseStep++;
      BtlNetSetBattleFlags(self, false);
    }
    if (self->phaseStep == 0x1a) {
      chara = NetCharaGetByIndex(0);
      if (chara != NULL) {
        NetCharaSetReady(chara);
      }
    }
    break;
  case 0x1b:
    loading = CoreTaskFind(0x2774);
    if (loading != NULL) {
      /* UiLoadingSetField(loading, 3, 4): close the loading screen */
      entry = &((const VtblEntry *)loading->base.vtable)[5];
      ((void (*)(void *, u32, u32))entry->fn)((char *)loading + entry->delta, 3, 4);
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
      GfxFaderStart(GfxGetActiveFader(), 20);
    }
    self->phaseStep++;
    /* fallthrough */
  case 0x1c:
    if (!GfxFaderIsFinished(GfxGetActiveFader())) {
      break;
    }
    if (UiLoadingIsOpen()) {
      break;
    }
    BtlMainUpdateScene(self);
    self->phaseStep++;
    /* fallthrough */
  case 0x1d:
    if (self->field8 != 0) {
      break;
    }
    ok = true;
    if (self->rematch != 0) {
      ok = false;
    }
    if (self->skipAppearDemo != 0) {
      ok = false;
    }
    if (SaveGetProfileFlag0() && SaveHasProfile() && SaveProfileHasFlags(SaveGetProfile(), 0x4880)) {
      ok = false;
    }
    if (ok) {
      BtlMainStartAppearDemo(self);
    }
    self->phaseStep++;
    /* fallthrough */
  case 0x1e:
    if (CoreTaskExists(0x67)) {
      break;
    }
    if (!SaveGetProfileFlag0() || !SaveProfileHasFlags(SaveGetProfile(), 0x80)) {
      SndBgmQueuePreload(0, BtlMainGetBgmId(self));
    }
    for (i = 0; i < 8; i++) {
      ActorStageObjUpdateAll();
    }
    self->dimColor[0] = 0.0f;
    self->dimColor[1] = 0.0f;
    self->dimColor[2] = 0.0f;
    self->dimColor[3] = 1.0f;
    GfxCameraStopShake(g_gfxActiveCamera);
    self->phaseStep++;
    display = g_gfxDisplay;
    clearColor = BtlStageGetClearColor();
    /* 16-byte copy (lv.q/sv.q in the binary); g_gfxDisplay is read before the call */
    display->clearColor[0] = clearColor[0];
    display->clearColor[1] = clearColor[1];
    display->clearColor[2] = clearColor[2];
    display->clearColor[3] = clearColor[3];
    g_gfxDisplay->frameSkip = 1;
    if (!SaveGetProfileFlag0()) {
      g_btlItemSpawnEnabled = 1;
    }
    /* fallthrough */
  case 0x1f:
    if (SaveGetProfileFlag0() && SaveProfileHasFlags(SaveGetProfile(), 0x80)) {
      self->phaseStep++;
      break;
    }
    if (CoreTaskExists(0x2756)) {
      break;
    }
    if (!SndStreamFileIsLoaded(BtlMainGetBgmId(self))) {
      break;
    }
    SndBgmQueuePlay(0, BtlMainGetBgmId(self), 1, 0);
    self->phaseStep++;
    break;
  default:
    if (!NetPlayHasManager()) {
      unit = BtlGetPlayerBakugan();
      if (unit != NULL) {
        unit->input->queuedActions = 0x40;
      }
    }
    self->rematch = 0;
    self->keepRounds = 0;
    self->roundRecorded = 0;
    self->winsCounted = 0;
    self->phaseStep = 0;
    self->standingTargetLimit = ActorStageObjCountStandingTargets();
    UiSetWindowActive(0, 1);
    phase = 1;
    if (BtlIsStoryBattle()) {
      phase = 9;
    }
    self->phase = phase;
    self->drawPhase = phase;
    break;
  }
}
