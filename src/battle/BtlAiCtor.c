// bdc 0x0888c3c8 BtlAiCtor
#include "bdc.h"

/* Base constructor of the CPU AI object (`BtlAi`, 0xa30 bytes); returns `self`.
   Builds the five embedded behaviour layers: each first gets the base `BtlAiLayer` state (vtable
   `g_btlAiLayerVtbl`, null methods `g_btlAiLayerNullMethods`, timers cleared and expired), then
   its {check, run} pair from `g_btlAiLayerMethods`. Guard, follow and seek-item also install their
   own vtable (`g_btlAiGuardLayerVtbl`, `g_btlAiFollowLayerVtbl`, `g_btlAiSeekItemLayerVtbl`)
   and get an inlined reset (state, flags and command cleared, own field cleared); the wander layer
   keeps the base vtable and is reset through vtable slot 1 (`BtlAiLayerReset`); the think layer
   gets no reset. Then sets the defaults (`ownerId` -1, `level` 5, `activeLayer` 4, `pendingHit` -1,
   everything else 0), constructs the virtual pad (`BtlAiPadCtor`), the four command channels
   (`CxxVecNewSimple` with `BtlAiChannelCtor`) and the combo table (`BtlAiComboTableCtor`),
   gives each delay `BtlAiWeightTable` a zeroed 6-word array from the low end of the heap,
   clears the channel states (`BtlAiChannelClearState`), `layerOrder` and the target pickers
   (`g_btlAiNullTargetPicker`), zeroes `lastPos` and `goal` (the binary stores the VFPU bank
   constant C720 = (0,0,0,0)), restarts the start timer at 60.0 and acquires the rule-script cache
   (`BtlAiComScriptCacheAcquire`). */

/* Base BtlAiLayer constructor, inlined five times. */
static void BtlAiCtorLayerBase(BtlAiLayer *layer)
{
  layer->vtbl = g_btlAiLayerVtbl;
  layer->check = g_btlAiLayerNullMethods[0];
  layer->run = g_btlAiLayerNullMethods[1];
  layer->flags = 0;
  layer->timerLimit = 0.0f;
  layer->timerElapsed = 0.0f;
  layer->timerExpired = 1;
  layer->cmd.timerLimit = 0.0f;
  layer->cmd.timerElapsed = 0.0f;
  layer->cmd.timerExpired = 1;
  layer->cmd.flags = 0;
  layer->state = 0;
  layer->active = 0;
  layer->cmd.state = 0;
  layer->cmd.arg = 0;
  layer->cmd.argF = 0.0f;
  layer->cmd.finished = 0;
  layer->cmd.failed = 0;
}

/* Reset inlined in the guard / follow / seek-item constructors (layer timer left alone). */
static void BtlAiCtorLayerReset(BtlAiLayer *layer)
{
  layer->state = 0;
  layer->active = 0;
  layer->flags = 0;
  layer->cmd.state = 0;
  layer->cmd.timerLimit = 0.0f;
  layer->cmd.timerElapsed = 0.0f;
  layer->cmd.timerExpired = 1;
  layer->cmd.flags = 0;
  layer->cmd.arg = 0;
  layer->cmd.argF = 0.0f;
  layer->cmd.finished = 0;
  layer->cmd.failed = 0;
}

/* Weight-table constructor followed by an inlined resize to 6 zeroed entries. */
static void BtlAiCtorWeightTable(BtlAiWeightTable *table)
{
  s32 *weights;
  bool fromLow;
  u32 i;

  table->weights = NULL;
  table->count = 0;
  table->borrowed = 0;
  table->total = 0;
  table->totalValid = 0;
  if (table->borrowed != 0) {
    table->weights = NULL;
    table->borrowed = 0;
  }
  weights = table->weights;
  if (weights != NULL) {
    MemLock();
    MemFree(weights, NULL, 0);
    MemUnlock();
    table->weights = NULL;
  }
  table->total = 0;
  table->totalValid = 0;
  table->count = 6;
  MemLock();
  fromLow = MemIsAllocFromLow();
  MemSetAllocFromLow(true);
  weights = MemAlloc(6 * sizeof(s32), NULL, 0);
  MemSetAllocFromLow(fromLow);
  MemUnlock();
  table->weights = weights;
  for (i = 0; i < table->count; i++) {
    table->weights[i] = 0;
  }
}

BtlAi *BtlAiCtor(BtlAi *self)
{
  const VtblEntry *entry;
  s32 i;

  /* guard layer */
  BtlAiCtorLayerBase(&self->guard.base);
  self->guard.base.vtbl = g_btlAiGuardLayerVtbl;
  self->guard.base.check = g_btlAiLayerMethods[0];
  self->guard.base.run = g_btlAiLayerMethods[1];
  BtlAiCtorLayerReset(&self->guard.base);
  self->guard.counterArmed = 0;

  /* follow layer */
  BtlAiCtorLayerBase(&self->follow.base);
  self->follow.base.vtbl = g_btlAiFollowLayerVtbl;
  self->follow.leader = NULL;
  self->follow.base.check = g_btlAiLayerMethods[2];
  self->follow.base.run = g_btlAiLayerMethods[3];
  BtlAiCtorLayerReset(&self->follow.base);
  self->follow.leader = NULL;

  /* seek-item layer */
  BtlAiCtorLayerBase(&self->seekItem.base);
  self->seekItem.base.vtbl = g_btlAiSeekItemLayerVtbl;
  self->seekItem.item = NULL;
  self->seekItem.base.check = g_btlAiLayerMethods[4];
  self->seekItem.base.run = g_btlAiLayerMethods[5];
  BtlAiCtorLayerReset(&self->seekItem.base);
  self->seekItem.item = NULL;

  /* wander layer: keeps the base vtable, reset through vtable slot 1 */
  BtlAiCtorLayerBase(&self->wander.base);
  self->wander.base.check = g_btlAiLayerMethods[6];
  self->wander.base.run = g_btlAiLayerMethods[7];
  entry = &self->wander.base.vtbl[1];
  ((void (*)(void *))entry->fn)((u8 *)&self->wander + entry->delta);

  /* think layer */
  BtlAiCtorLayerBase(&self->think);
  self->think.check = g_btlAiLayerMethods[8];
  self->think.run = g_btlAiLayerMethods[9];

  self->ownerId = -1;
  self->owner = NULL;
  self->level = 5;
  self->targetStyle = 0;
  self->targetStyleArg = 0;
  BtlAiPadCtor(&self->pad);
  self->ruleMode = 0;
  self->scriptCache = NULL;
  self->script = NULL;
  self->params = NULL;
  self->delaySet0 = NULL;
  self->delaySet1 = NULL;
  CxxVecNewSimple(self->channels, 4, sizeof(BtlAiChannel), BtlAiChannelCtor);
  BtlAiComboTableCtor(&self->comboTable);
  self->guardRoll = 0;
  self->dodgePressed = 0;
  self->allowedCmds = 0;
  self->activeLayer = 4;
  self->idleTime = 0.0f;
  self->firstFrame = 0;
  self->startLimit = 0.0f;
  self->startElapsed = 0.0f;
  self->startExpired = 1;
  self->chargeCooldown = 0;
  self->target = NULL;
  self->savedTarget = NULL;
  self->ctorZero974[0] = 0.0f;
  self->ctorZero974[1] = 0.0f;
  self->targetHeight = 0.0f;
  self->targetIdleFrames = 0.0f;
  self->odometer = 0.0f;
  self->pendingHit = -1;
  self->ctorZero9a8 = 0.0f;
  self->moveFlags = 0;
  self->threat = NULL;
  self->incomingAttack = NULL;
  self->guardDelayChance = 0;
  self->seekItemChance = 0;
  self->waitChance = 0;
  self->counterChance = 0;
  self->slot6Chance = 0;
  self->comboFalloff = 0.0f;
  self->guardDelayMin = 0.0f;
  self->guardDelayMax = 0.0f;
  BtlAiCtorWeightTable(&self->delayWeights[0]);
  BtlAiCtorWeightTable(&self->delayWeights[1]);
  self->forwardDashFrames = 0;
  self->targetChanged = 0;

  for (i = 0; i < 4; i++) {
    BtlAiChannelClearState(&self->channels[i]);
  }
  for (i = 0; i < 5; i++) {
    self->layerOrder[i] = NULL;
  }
  for (i = 0; i < 4; i++) {
    self->targetPickers[i] = g_btlAiNullTargetPicker;
  }

  /* sv.q of the bank constant C720 = (0, 0, 0, 0). */
  for (i = 0; i < 4; i++) {
    self->lastPos[i] = 0.0f;
  }
  for (i = 0; i < 4; i++) {
    self->goal[i] = 0.0f;
  }

  /* The binary copies a 12-byte stack timer {60.0f, 0.0f, 0} over +0x93c..+0x947, so the three
     padding bytes after startExpired get uninitialised stack bytes; only the fields are kept. */
  self->startLimit = 60.0f;
  self->startElapsed = 0.0f;
  self->startExpired = 0;
  self->scriptCache = BtlAiComScriptCacheAcquire();
  return self;
}
