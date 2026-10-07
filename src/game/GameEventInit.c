// bdc 0x088ef07c GameEventInit
#include "bdc.h"

/* Prepares the field event task (task id 470, `GameEvent470Ctor`, base `GameEventCtor`; message
   pack `mes_f<area>_<room>_<lang>.bin`, command script `cmd_f%d_%02d.cut`) for a command script:
   stores `mode`, allocates its work blocks from the low heap (the 4-byte `fades` block uncleared,
   the 4-byte `fadeWork` zeroed, the 0x68-byte camera key record `camKeys` cleared by
   `CxxZeroMemory`, ten 100-byte prop records `props` with the scales and `bobAmp` at 0x1000 (1.0 in
   20.12) and everything else zero, and the 0x54-byte action list `actions` built by
   `GameEventActionListCtor` and started), then looks the script `cutName` up in the pack chain
   `g_ioLzsPackages`: `script`, `entryCount` (its first word), `entries` (the table after it) and
   `commands` (after the table). Sets `state = 1`. */

void GameEventInit(GameEvent *self, s32 unused, u8 mode, const char *cutName)
{
  bool fromLow;
  GameEventFadeRecord *fades;
  u8 *fadeWork;
  GameEventCamKeys *camKeys;
  GameEventActionList *actions;
  GameEventPropRecord *rec;
  s32 *scales;
  s32 *script;
  u8 i;
  u8 j;

  self->mode = mode;

  MemLock();
  fromLow = MemIsAllocFromLow();
  MemSetAllocFromLow(true);
  fades = MemAlloc(4, NULL, 0);
  MemSetAllocFromLow(fromLow);
  MemUnlock();
  self->fades = fades;

  MemLock();
  fromLow = MemIsAllocFromLow();
  MemSetAllocFromLow(true);
  fadeWork = MemAlloc(4, NULL, 0);
  MemSetAllocFromLow(fromLow);
  MemUnlock();
  if (fadeWork != NULL) {
    fadeWork[0] = 0;
    fadeWork[1] = 0;
    fadeWork[2] = 0;
    fadeWork[3] = 0;
  }
  self->fadeWork = fadeWork;

  MemLock();
  fromLow = MemIsAllocFromLow();
  MemSetAllocFromLow(true);
  camKeys = MemAlloc(sizeof(GameEventCamKeys), NULL, 0);
  MemSetAllocFromLow(fromLow);
  MemUnlock();
  if (camKeys != NULL) {
    CxxZeroMemory(camKeys, sizeof(GameEventCamKeys));
  }
  self->camKeys = camKeys;

  MemLock();
  fromLow = MemIsAllocFromLow();
  MemSetAllocFromLow(true);
  rec = MemAlloc(10 * sizeof(GameEventPropRecord), NULL, 0);
  MemSetAllocFromLow(fromLow);
  MemUnlock();
  self->props = rec;
  for (i = 0; i < 10; i++) {
    memset(self->props[i].pos, 0, 0x30);  /* pos, endPos, startPos, _unk24 */
    memset(self->props[i].rot, 0, 0x18);  /* rot, endRot, startRot, _unk5e */
    self->props[i].prop = NULL;
    self->props[i].bobAmp = 0x1000;
    self->props[i].bobBaseY = 0;
    for (j = 0; j < 4; j++) {
      /* scale, endScale, startScale, baseScale */
      scales = &self->props[i].scale;
      scales[j] = 0x1000;
    }
  }

  MemLock();
  fromLow = MemIsAllocFromLow();
  MemSetAllocFromLow(true);
  actions = MemAlloc(sizeof(GameEventActionList), NULL, 0);
  MemSetAllocFromLow(fromLow);
  MemUnlock();
  if (actions != NULL) {
    GameEventActionListCtor(actions);
  }
  self->actions = actions;
  GameEventActionListStart(actions);

  script = CorePackChainFind(g_ioLzsPackages, (char *)cutName);
  self->script = script;
  self->entryCount = script[0];
  self->entries = (GameEventScriptEntry *)(script + 1);
  self->commands = (GameEventCommand *)(self->entries + self->entryCount);
  self->state = 1;
}
