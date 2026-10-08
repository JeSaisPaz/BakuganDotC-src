// bdc 0x088c5a20 GameFieldPhaseMain
#include "bdc.h"

/* Phase 1 (main loop) of the field (world map) scene task, task id 500 (`GameFieldCtor`, 0x7a0
   bytes): updates the world (`GameFieldUpdateWorld`) and runs the sub-state machine `subState`
   (+0x61c): 0 entry (BGM, return-location restore, HUD, voice, fade-in), 1/2 player input
   (`GameFieldHandleInput`), 5 waits for a field event and dispatches on
   `g_gameEventTransitionKind`, 6..10 / 13..17 area reload with fades, 11..12 the caught/talk view
   (alarm, look-at blend to the guard), 19..21 the event transition, 24..25 fade-out and leave
   (phase 3 with `nextRequest`), 28..29 the Marucho jet confirm dialog (`mes_MaruchoJet_%s.bin`).
   Afterwards updates the guard-blind timer and flushes queued party removals. */

typedef s32 (*GameFieldPartnerTalkFn)(void *self);
typedef void (*GameFieldPlayerVoidFn)(void *self);

void GameFieldPhaseMain(CoreTask *task)

{
  GameFieldTask *field = (GameFieldTask *)task;
  GameFieldCamera *cam = (GameFieldCamera *)field->camera;
  GameFieldPlacedChar *rec;
  GfxFader *fader;
  GfxFader *src;
  SaveProfile *profile;
  Actor *actor;
  Actor *partner;
  const VtblEntry *slot;
  u32 *table;
  u8 returned;
  u8 mode;
  s16 duration;
  s32 prevStage;
  s32 stage;
  s32 mapId;
  s32 i;
  float t;
  ScePspFVector4 tmp;
  u16 info[10];
  char name[96];

  GameFieldUpdateWorld(task);
  switch (field->subState) {
  case 0:
    GameFieldPlayStageBgm();
    field->subState = 1;
    returned = 0;
    mode = g_gameFieldEntryMode;
    if (mode == 1) {
      rec = ((GameFieldPlacedChar **)g_gameEventLocationBlock)[0];
      ((u32 *)g_gameUnlockFlags)[0] = rec->pos[0];
      ((u32 *)g_gameUnlockFlags)[1] = rec->pos[1];
      ((u32 *)g_gameUnlockFlags)[2] = rec->pos[2];
      ((s16 *)g_gameUnlockFlags)[6] = rec->rot[1];
    }
    else if (mode == 2) {
      profile = SaveGetProfile();
      if (profile->data->stageStates[g_scriptGlobalVars[1]] == 2 && g_gameTransitionParam != -1) {
        GameFieldReturnToLocation(task, g_gameTransitionParam, g_gameEventTransitionActor);
        returned = 1;
      }
      if (returned) {
        field->subState = 4;
      }
      else {
        rec = ((GameFieldPlacedChar **)g_gameEventLocationBlock)[0];
        ((u32 *)g_gameUnlockFlags)[0] = rec->pos[0];
        ((u32 *)g_gameUnlockFlags)[1] = rec->pos[1];
        ((u32 *)g_gameUnlockFlags)[2] = rec->pos[2];
        ((s16 *)g_gameUnlockFlags)[6] = rec->rot[1];
      }
    }
    else {
      if (g_gameFieldReturnKind == 1 || g_gameFieldReturnKind == 2 || g_gameFieldReturnKind == 3) {
        GameFieldRestoreReturnLocation(task);
        ActorApplyPlacement((Actor *)g_gameFieldCharSet->actors[0]);
        GameFieldCameraReset(cam, 1, 0);
        g_gameFieldReturnKind = 0;
      }
    }
    g_gameFieldEntryMode = 0;
    if (cam->questCam != NULL) {
      for (i = 0; i < 120; i++) {
        GameFieldCameraUpdateQuestCam(cam);
      }
    }
    if (!returned) {
      GameFieldOpenHud();
      ((ActorPlayer *)ActorFindPlayer())->promptA = 1;
      SndBgmPlayVoice(field->stageVoiceId);
      field->hudOpenedOnEntry = 1;
    }
    fader = GfxGetActiveFader();
    if (!(fader->color[3] <= 0.0f)) {
      fader = GfxGetActiveFader();
      src = GfxGetActiveFader();
      fader->start[0] = src->color[0];
      fader->start[1] = src->color[1];
      fader->start[2] = src->color[2];
      fader->start[3] = src->color[3];
      fader = GfxGetActiveFader();
      fader->end[0] = 0.0f;
      fader->end[1] = 0.0f;
      fader->end[2] = 0.0f;
      fader->end[3] = 0.0f;
      GfxFaderStart(GfxGetActiveFader(), 20);
    }
    break;
  case 1:
    if (GfxFaderIsFinished(GfxGetActiveFader())) {
      if (field->hudOpenedOnEntry != 0) {
        field->hudOpenedOnEntry = 0;
      }
      else {
        UiFieldHudRequestReset();
        GameFieldOpenHud();
      }
      GameFieldCharSetRestartAll(field->charSet);
      field->subState++;
    }
    /* fall through */
  case 2:
    field->subState = GameFieldHandleInput(task);
    break;
  case 3:
    break;
  case 4:
    if (GfxFaderIsFinished(GfxGetActiveFader())) {
      field->subState++;
    }
    break;
  case 5:
    if (GameEventIsBusy(&field->events->base)) {
      if (field->pulseActive != 0) {
        GameFieldPulseSpriteUpdate(task);
      }
      break;
    }
    field->pulseActive = 0;
    ActorBallSetList(field->ballList);
    if (field->itemBoxOpened != 0) {
      field->itemBoxOpened = 0;
      field->subState = 0x1f;
      break;
    }
    /* fall through */
  case 0x20:
    switch (g_gameEventTransitionKind) {
    case 0:
      GameFieldEndEventView(task);
      if (field->returnedFlag == 0) {
        GameFieldOpenHud();
      }
      field->subState = 2;
      break;
    case 1:
      g_gameFieldEntryMode = 2;
      GameFieldSaveReturnLocationAndLeave(task);
      field->subState = 0x18;
      break;
    case 2:
      field->leaveRequest = 10;
      field->subState = 0x18;
      break;
    case 3:
      g_gameFieldEntryMode = 1;
      field->leaveRequest = 10;
      field->subState = 0x18;
      break;
    case 4:
      field->leaveRequest = 3;
      field->subState = 0x18;
      break;
    case 5:
      g_gameFieldEntryMode = 1;
      field->leaveRequest = 0xc;
      field->subState = 0x18;
      break;
    case 6:
      g_gameFieldEntryMode = 1;
      field->leaveRequest = 0x10;
      field->subState = 0x18;
      break;
    }
    break;
  case 6:
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
    field->subState++;
    break;
  case 7:
    if (GfxFaderIsFinished(GfxGetActiveFader())) {
      GameFieldUnloadArea(field, true);
      field->subState++;
    }
    break;
  case 8:
    GameFieldEnterArea(task, true);
    field->subState++;
    break;
  case 9:
    GameFieldCameraReset(cam, 1, 0);
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
    field->subState++;
    break;
  case 10:
    field->subState = 1;
    break;
  case 11:
    /* caught: frame the catching guard and let the alarm ring */
    field->eventView = 1;
    field->eventViewTimer = (g_gameEventFlags[5] != 0) ? 50 : 90;
    field->alarmTimer = 60;
    if (field->caughtSlot == 0xff) {
      field->talkPartner = NULL;
    }
    else {
      partner = (Actor *)g_gameFieldCharSet->actors[field->caughtSlot];
      field->talkPartner = partner;
      GameFieldCameraBeginTalkViewWith(cam, partner);
      slot = &((const VtblEntry *)partner->base.base.vtable)[12];
      if (((GameFieldPartnerTalkFn)slot->fn)((u8 *)partner + slot->delta) != 0) {
        duration = (g_gameEventFlags[5] != 0) ? 70 : 130;
      }
      else {
        duration = (g_gameEventFlags[5] != 0) ? 60 : 110;
      }
      field->eventViewTimer = duration;
    }
    field->alarmActive = 1;
    field->subState++;
    break;
  case 12:
    partner = field->talkPartner;
    if (partner != NULL &&
        (field->eventViewTimer < g_gameFieldTalkBlendFrames || field->talkBlendForce != 0)) {
      /* talkLookAt += (partner position - talkLookAt) * talkBlendRate */
      t = field->talkBlendRate;
      tmp = cam->talkLookAt;
      tmp.x = tmp.x + (partner->base.pos[0] - tmp.x) * t;
      tmp.y = tmp.y + (partner->base.pos[1] - tmp.y) * t;
      tmp.z = tmp.z + (partner->base.pos[2] - tmp.z) * t;
      tmp.w = tmp.w + (partner->base.pos[3] - tmp.w) * t;
      cam->talkLookAt = tmp;
      if (g_gameFieldTalkEyeMaxY < cam->talkEye.y) {
        t = field->talkBlendRate;
        cam->talkEye.y = cam->talkEye.y * (1.0f - t) + g_gameFieldTalkEyeMaxY * t;
      }
    }
    if (field->eventViewTimer <= 0 || --field->alarmTimer < 0) {
      if (SndHasManager()) {
        SndManagerStop(SndGetManager(), field->alarmLoopHandle);
      }
      field->alarmTimer = 0;
      if (field->alarmActive != 0) {
        field->alarmActive = 0;
      }
    }
    if (--field->eventViewTimer <= 0) {
      field->talkBlendForce = 0;
      field->subState++;
    }
    break;
  case 13:
    if (g_gameEventFlags[5] != 0) {
      field->eventViewTimer = 8;
      field->subState = 0x13;
    }
    else {
      field->talkPartner = NULL;
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
      GfxFaderStart(GfxGetActiveFader(), 10);
      field->subState++;
    }
    break;
  case 14:
    if (GfxFaderIsFinished(GfxGetActiveFader())) {
      if (g_gameFieldCharSet->paused != 0) {
        g_gameFieldCharSet->paused = 0;
      }
      ActorPlayerResetPowers((ActorPlayer *)ActorFindPlayer());
      GameFieldUnloadArea(field, false);
      field->subState++;
    }
    break;
  case 15:
    GameFieldEnterArea(task, false);
    for (i = 0; i < 120; i++) {
      GameFieldCameraSnapQuestCam(cam);
      GameFieldCameraUpdateQuestCam(cam);
    }
    field->subState++;
    break;
  case 16:
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
    GfxFaderStart(GfxGetActiveFader(), 10);
    GameFieldCameraReset(cam, 1, 0);
    GameFieldCameraUpdate(cam);
    field->eventView = 0;
    field->subState = 1;
    break;
  case 17:
    if (GfxFaderIsFinished(GfxGetActiveFader())) {
      field->subState = 2;
    }
    break;
  case 18:
    actor = (Actor *)ActorFindPlayer();
    if (actor->state != 7) {
      GameFieldCharSetFreezeAll(field->charSet);
      GameFieldStartEvent(task, field->caughtEventId, 0, 0, false);
      GameFieldCloseHud();
      field->subState = 5;
    }
    break;
  case 19:
    if (--field->eventViewTimer <= 0) {
      GameFieldPlayTransition(task, 1);
      field->subState++;
    }
    break;
  case 20:
    if (!GameFieldPlayTransition(task, 0)) {
      if (field->caughtSlot != 0xff) {
        GameFieldStartEvent(task, field->caughtEventId, 0,
                            g_gameFieldCharSet->events[0x20 + field->caughtSlot], false);
      }
      field->subState++;
    }
    break;
  case 21:
    if (!GameEventIsBusy(&field->events->base)) {
      g_gameEventFlags[5]--;
      profile = SaveGetProfile();
      profile->data->fieldCounter--;
      if (profile->data->fieldCounter < 0) {
        profile->data->fieldCounter = 0;
      }
      actor = (Actor *)ActorFindPlayer();
      if (actor != NULL) {
        slot = &((const VtblEntry *)actor->base.base.vtable)[13];
        ((GameFieldPlayerVoidFn)slot->fn)((u8 *)actor + slot->delta);
      }
      if (field->caughtSlot != 0xff && field->talkPartner != NULL) {
        ActorNpcEnterState11((ActorNpc *)field->talkPartner);
      }
      if (SaveHasProfile()) {
        GameFieldGuardBlindStart(&field->guardBlind);
      }
      GameFieldEndEventView(task);
      GameFieldOpenHud();
      GameFieldResetGimmickStates(task, false);
      field->subState++;
    }
    break;
  case 22:
    field->subState = 2;
    break;
  case 23:
    actor = (Actor *)ActorFindPlayer();
    if (actor != NULL && actor->state == 0xc) {
      break;
    }
    GameFieldStartEvent(task, field->caughtEventId, 0, 0, false);
    field->subState = 5;
    break;
  case 24:
    GfxGetActiveFader()->sortKey = 20000.0f;
    fader = GfxGetActiveFader();
    src = GfxGetActiveFader();
    fader->start[0] = src->color[0];
    fader->start[1] = src->color[1];
    fader->start[2] = src->color[2];
    fader->start[3] = src->color[3];
    fader = GfxGetActiveFader();
    fader->end[0] = 0.0f;
    fader->end[1] = 0.0f;
    fader->end[2] = 0.0f;
    fader->end[3] = 1.0f;
    GfxFaderStart(GfxGetActiveFader(), 20);
    field->subState++;
    break;
  case 25:
    if (!GfxFaderIsFinished(GfxGetActiveFader())) {
      break;
    }
    switch (field->leaveRequest) {
    case 3:
      field->nextRequest = 5;
      field->phase = 3;
      field->subState = 0;
      break;
    case 10:
      g_scriptGlobalVars[1] = 0x20;
      field->phase = 3;
      field->subState = 0;
      break;
    case 0xc:
      g_scriptGlobalVars[1] = g_gameEventFlags[0] * 4 + g_gameEventFlags[2];
      field->phase = 3;
      field->subState = 0;
      break;
    case 0xe:
      prevStage = g_scriptGlobalVars[1];
      field->nextRequest = 2;
      stage = g_scriptGlobalVars[1];
      GameStageGetInfo(info, (u8)(stage / 4), (u8)(stage % 4));
      if (info[1] == 1 || !CoreBitsetTest(0x1c, g_scriptGlobalBits)) {
        field->nextRequest = 8;
      }
      SaveProfileClearPlacedHolograms();
      SaveProfileSetMapMode(2, (u8)g_scriptGlobalVars[1]);
      profile = SaveGetProfile();
      SaveProfileSetWord(profile, 0x33, g_scriptGlobalVars[1]);
      SaveProfileSetWord(SaveGetProfile(), 0x2e, 0);
      SaveProfileSetWord(SaveGetProfile(), 0x31, prevStage);
      if (g_scriptGlobalVars[1] == 0x25 && CoreBitsetTest(0x1d, g_scriptGlobalBits)) {
        g_scriptGlobalVars[15] = 0x11;
      }
      GameFieldRestoreStage20EventWord();
      field->phase = 3;
      field->subState = 0;
      break;
    case 0x10:
      field->nextRequest = 10;
      field->phase = 3;
      field->subState = 0;
      break;
    }
    break;
  case 26:
    if (--field->eventViewTimer < 0) {
      field->subState++;
    }
    break;
  case 27:
    GameFieldSaveReturnLocation();
    field->nextRequest = 9;
    g_gameFieldReturnKind = 1;
    field->phase = 3;
    field->subState = 0;
    break;
  case 28:
    CoreTaskCreate(0x1fe, 100);
    sprintf(name, "mes_MaruchoJet_%s.bin", SaveGetLanguageName());
    table = CorePackChainFind(g_ioLzsPackages, name);
    UiMesTableRelocate(table);
    UiConfirmDialogSetMessage((const char *)PspPtr(table[2]));
    field->subState++;
    break;
  case 29:
    if (CoreTaskExists(0x1fe)) {
      break;
    }
    if (UiConfirmDialogGetResult() == 1) {
      GameFieldSaveReturnLocation();
      if (CoreBitsetTest(0x18, g_scriptGlobalBits)) {
        field->nextRequest = 5;
      }
      /* fly to the last jet destination whose flag is set */
      for (i = 16; i >= 0; i--) {
        if (CoreBitsetTest(g_gameFieldJetStageTable[i][0], g_scriptGlobalBits)) {
          g_scriptGlobalVars[1] = g_gameFieldJetStageTable[i][1];
          break;
        }
      }
      profile = SaveGetProfile();
      profile->data->stageProfileByte = (u8)(g_scriptGlobalVars[1] / 4);
      profile = SaveGetProfile();
      profile->data->stageSlot = (u8)(g_scriptGlobalVars[1] % 4);
      mapId = GameStageToMapId(g_scriptGlobalVars[1]);
      g_scriptGlobalVars[15] = mapId;
      if (GameFieldIsMapMovieSet1Watched()) {
        GameFieldCommitStageProgress(task);
      }
      else {
        g_gameFieldEntryMode = 1;
      }
      UiFieldHudDisable();
      field->phase = 3;
      field->subState = 0;
    }
    else {
      GameFieldCameraReset(cam, 0, 0);
      GameFieldCharSetRestartAll(field->charSet);
      field->subState = 2;
    }
    break;
  case 30:
    if (field->itemBoxOpened != 0) {
      GameFieldStartEvent(task, field->caughtEventId, 0, 0, false);
      field->subState = 5;
    }
    break;
  case 31:
    if (SaveProfileHasFlags(SaveGetProfile(), 0x20000000)) {
      field->subState = 0x20;
    }
    break;
  case 33:
    actor = (Actor *)ActorFindPlayer();
    if (actor->state != 0xb) {
      field->subState = 2;
    }
    break;
  }
  if (SaveHasProfile()) {
    GameFieldGuardBlindUpdate(&field->guardBlind.state);
  }
  GameFieldPartyFlushRemovals(task);
}
