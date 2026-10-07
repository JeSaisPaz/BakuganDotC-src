// bdc 0x088c3e6c GameFieldHandleInput
#include "bdc.h"

/* Player interaction step of the field (world map) scene task, task id 500 (`GameFieldCtor`,
   0x7a0 bytes, vtable `0x08af2cfc`) (called by `GameFieldPhaseMain`). Returns the next sub-state;
   `subState` (`+0x61c`) unchanged when nothing happens (also when there is no pad).
   - `eventViewTimer` > 0: counts it down and returns.
   - START (pressed 0x8): opens the pause menu (`GameFieldOpenPauseMenu`), mode 5 in area 8, else 4.
   - Player caught (`GameFieldCheckPlayerCaught`): 0xb.
   - A gimmick from `GameFieldCheckTriggers`, by its virtual slots and kind/type:
     - slot 10 or 14: freezes all characters, cancels the throw of character 0, takes the event id of
       its entry (`GameGimmickGetRecordParam`, `GameFieldGetEventEntry`) into `caughtEventId`, puts the
       player in state 0xc with the gimmick as `talkTarget` unless `triggerByContact`, closes the HUD: 0x17;
     - kind 9: profile word 0x34 = 1, same but player state 0xa and `eventView` = 1: 0x1e;
     - slot 13: `caughtEventId` from its entry: 0x12;
     - kind 0x10 / type 0x322: (kind 0x10 plays sound 0) freeze, camera hold, saves the return location,
       `nextRequest` 0xb, `g_gameFieldReturnKind` 2, `leaveRequest` 10: 0x18;
     - kind 0x11 / type 0x325: (kind 0x11 plays sound 0) freeze, camera hold: 0x1c; type 0x323: 0x1a;
     - type 0x324: sound 0, freeze, return location, `nextRequest` 0xc, return kind 3, `leaveRequest` 10,
       clears the placed holograms, map mode 1 to the current stage (script var 1), profile words
       0x33 = stage, 0x2e = 0, 0x31 = stage: 0x18;
     - type 6000 (door): area block `g_gameEventFlags[2]` and `g_gameFieldEntryId` from the record's
       `entrySlot` nibbles, freezes character 0: 6;
     - type 0x1773/0x1774 (event point, not the current `setupGimmick`): when the entry's flag (`+2`) is 0
       or clear in the story bitset, starts its event (`GameFieldStartEvent`); a 0x1773 setup-only entry
       (`GameEvent470IsSetupOnlyEntry`) keeps the balls, becomes `setupGimmick` and sets the player's
       `focusPoint` to the record's `eventFocus` x 20 (w lane left out: stale VFPU lane); otherwise freeze, close HUD
       and 5. Except in the setup-only case it then calls the gimmick's virtual slot 16 with 0.
   - No gimmick: with a talk target (`ActorPlayerFindTalkTarget`) and the action button (0x4000),
     copies its event id/slot into the player's placement record, cancels the throw
     (`ActorPlayerDropBall`), freezes all and starts the event of location slot 0, closing the HUD (5).
     Then 0x21 when the player is in state 0xb.
   VFPU: focusPoint.w gets the stale S713 lane on the PSP; the C leaves it unchanged. */

u32 GameFieldHandleInput(CoreTask *task)
{
  GameFieldTask *field = (GameFieldTask *)task;
  PadState *pad;
  ActorPlayer *player;
  ActorPlayer *actor;
  Actor *talk;
  GameGimmick *g;
  const VtblEntry *e;
  GameGimmickRecord *rec;
  GameFieldPlacedChar *loc;
  SaveProfile *profile;
  u16 *entry;
  u32 *bits;
  u32 ret;
  s32 flag;
  s32 stage;
  s16 eventId;
  bool run;
  bool notify;
  float v[3];

  ret = field->subState;
  pad = g_padState;
  if (pad == NULL) {
    return ret;
  }
  player = (ActorPlayer *)ActorFindPlayer();
  if (field->eventViewTimer > 0) {
    field->eventViewTimer = field->eventViewTimer - 1;
    return ret;
  }
  if ((pad->pressed & 8) != 0) {
    if (g_gameEventFlags[0] == 8) {
      GameFieldOpenPauseMenu(task, 5);
    } else {
      GameFieldOpenPauseMenu(task, 4);
    }
    return ret;
  }
  if (GameFieldCheckPlayerCaught(task) != 0) {
    return 0xb;
  }

  g = GameFieldCheckTriggers(task);
  if (g == NULL) {
    talk = (Actor *)ActorPlayerFindTalkTarget(player);
    if (talk != NULL && (pad->pressed & 0x4000) != 0) {
      ((GameFieldPlacedChar *)player->base.placement)->eventId =
          ((GameFieldPlacedChar *)talk->placement)->eventId;
      ((GameFieldPlacedChar *)player->base.placement)->byte3a = talk->placementSlot;
      ActorPlayerDropBall(player);
      GameFieldCharSetFreezeAll(field->charSet);
      loc = *(GameFieldPlacedChar **)g_gameEventLocationBlock;
      GameFieldStartEvent(task, loc->eventId, 0, loc->byte3a, false);
      GameFieldCloseHud();
      ret = 5;
    }
    if (player->base.state == 0xb) {
      ret = 0x21;
    }
    return ret;
  }

  if ((e = &((const VtblEntry *)g->base.base.vtable)[10],
       ((int (*)(void *))e->fn)((char *)g + e->delta) != 0) ||
      (e = &((const VtblEntry *)g->base.base.vtable)[14],
       ((int (*)(void *))e->fn)((char *)g + e->delta) != 0)) {
    GameFieldCharSetFreezeAll(field->charSet);
    ActorPlayerDropBall((ActorPlayer *)g_gameFieldCharSet->actors[0]);
    entry = (u16 *)GameFieldGetEventEntry(task, GameGimmickGetRecordParam(g));
    field->caughtEventId = (s16)entry[0];
    if (field->triggerByContact == 0) {
      actor = (ActorPlayer *)ActorFindPlayer();
      if (actor != NULL) {
        ActorSetState(&actor->base, 0xc, 0);
        actor->talkTarget = g;
      }
    }
    GameFieldCloseHud();
    return 0x17;
  }

  if (g->kind == 9) {
    SaveProfileSetWord(SaveGetProfile(), 0x34, 1);
    GameFieldCharSetFreezeAll(field->charSet);
    ActorPlayerDropBall((ActorPlayer *)g_gameFieldCharSet->actors[0]);
    entry = (u16 *)GameFieldGetEventEntry(task, GameGimmickGetRecordParam(g));
    field->caughtEventId = (s16)entry[0];
    GameFieldCloseHud();
    actor = (ActorPlayer *)ActorFindPlayer();
    if (actor != NULL) {
      ActorSetState(&actor->base, 10, 0);
      actor->talkTarget = g;
    }
    field->eventView = 1;
    return 0x1e;
  }

  e = &((const VtblEntry *)g->base.base.vtable)[13];
  if (((int (*)(void *))e->fn)((char *)g + e->delta) != 0) {
    entry = (u16 *)GameFieldGetEventEntry(task, GameGimmickGetRecordParam(g));
    field->caughtEventId = (s16)entry[0];
    return 0x12;
  }

  if (g->kind == 0x10) {
    if (SndHasManager()) {
      SndManagerPlay(SndGetManager(), 0, 0, 0);
    }
    GameFieldCharSetFreezeAll(field->charSet);
    GameFieldCameraBeginHold((GameFieldCamera *)field->camera);
    GameFieldSaveReturnLocation();
    field->nextRequest = 0xb;
    g_gameFieldReturnKind = 2;
    field->leaveRequest = 10;
    return 0x18;
  }
  if (g->kind == 0x11) {
    if (SndHasManager()) {
      SndManagerPlay(SndGetManager(), 0, 0, 0);
    }
    GameFieldCharSetFreezeAll(field->charSet);
    GameFieldCameraBeginHold((GameFieldCamera *)field->camera);
    return 0x1c;
  }

  e = &((const VtblEntry *)g->base.base.vtable)[12];
  if (((int (*)(void *))e->fn)((char *)g + e->delta) == 0) {
    return ret;
  }

  switch (g->typeId) {
  case 0x322:
    GameFieldCharSetFreezeAll(field->charSet);
    GameFieldCameraBeginHold((GameFieldCamera *)field->camera);
    GameFieldSaveReturnLocation();
    field->nextRequest = 0xb;
    g_gameFieldReturnKind = 2;
    field->leaveRequest = 10;
    return 0x18;
  case 0x323:
    GameFieldCharSetFreezeAll(field->charSet);
    GameFieldCameraBeginHold((GameFieldCamera *)field->camera);
    return 0x1a;
  case 0x324:
    if (SndHasManager()) {
      SndManagerPlay(SndGetManager(), 0, 0, 0);
    }
    GameFieldCharSetFreezeAll(field->charSet);
    GameFieldSaveReturnLocation();
    field->nextRequest = 0xc;
    g_gameFieldReturnKind = 3;
    field->leaveRequest = 10;
    stage = g_scriptGlobalVars[1];
    SaveProfileClearPlacedHolograms();
    SaveProfileSetMapMode(1, (u8)g_scriptGlobalVars[1]);
    profile = SaveGetProfile();
    SaveProfileSetWord(profile, 0x33, (u32)g_scriptGlobalVars[1]);
    SaveProfileSetWord(SaveGetProfile(), 0x2e, 0);
    SaveProfileSetWord(SaveGetProfile(), 0x31, (u32)stage);
    return 0x18;
  case 0x325:
    GameFieldCharSetFreezeAll(field->charSet);
    GameFieldCameraBeginHold((GameFieldCamera *)field->camera);
    return 0x1c;
  case 6000:
    g_gameEventFlags[2] = (u8)(((GameGimmickRecord *)g->record)->entrySlot & 0xf);
    g_gameFieldEntryId = (s8)((((GameGimmickRecord *)g->record)->entrySlot & 0xf0) >> 4);
    GameFieldCharSetFreezeActor(field->charSet, 0);
    return 6;
  case 0x1773:
  case 0x1774:
    break;
  default:
    return ret;
  }

  /* event point (0x1773/0x1774) */
  if (field->setupGimmick == g) {
    return ret;
  }
  entry = (u16 *)GameFieldGetEventEntry(task, GameGimmickGetRecordParam(g));
  run = true;
  if (entry[1] != 0) {
    entry = (u16 *)GameFieldGetEventEntry(task, GameGimmickGetRecordParam(g));
    flag = entry[1];
    bits = (u32 *)&g_gameEventFlags[8];
    if (((bits[flag / 32] >> (flag % 32)) & 1) != 0) {
      run = false;
    }
  }
  notify = true;
  if (run) {
    entry = (u16 *)GameFieldGetEventEntry(task, GameGimmickGetRecordParam(g));
    eventId = (s16)entry[0];
    if (g->typeId == 0x1773 && GameEvent470IsSetupOnlyEntry(field->events, entry[0]) != 0) {
      GameFieldStartEvent(task, eventId, 0, 0, true);
      notify = false;
      field->setupGimmick = g;
      actor = (ActorPlayer *)ActorFindPlayer();
      rec = (GameGimmickRecord *)g->record;
      v[0] = (float)rec->eventFocus[0] * 0.000244140625f;
      v[1] = (float)rec->eventFocus[1] * 0.000244140625f;
      v[2] = (float)rec->eventFocus[2] * 0.000244140625f;
      /* vscl.t + sv.q: lanes x, y, z; lane w is a stale VFPU value, left out */
      actor->focusPoint[0] = v[0] * 20.0f;
      actor->focusPoint[1] = v[1] * 20.0f;
      actor->focusPoint[2] = v[2] * 20.0f;
    } else {
      GameFieldStartEvent(task, eventId, 0, 0, false);
      GameFieldCharSetFreezeAll(field->charSet);
      GameFieldCloseHud();
      ret = 5;
    }
  }
  if (notify) {
    e = &((const VtblEntry *)g->base.base.vtable)[16];
    ((void (*)(void *, s32))e->fn)((char *)g + e->delta, 0);
  }
  return ret;
}
