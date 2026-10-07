// bdc 0x08850e3c BtlMainTaskCtor
#include "bdc.h"

/* Constructor of the main battle-scene task (task id 100 = 0x64, 0x5b0 bytes, vtable `0x08af18f4`;
   the task `BtlCameraTaskExists`/`BtlGetCameraTask` look up): builds the follow camera at
   `+0x20` (`BtlCameraCtor`), records 100 in `g_lastScreenTaskId`, sets bit 0x22 and clears bits
   0x1f/0x23 of the script flag bitset; outside netplay clears profile word 0x13, forces the CPU to
   333 MHz (`CorePowerSetCpuClock`) and requests the crystal spawn, in netplay resets the net
   character sync. Creates three sprite layers from the low heap (2D, then two 3D ones published
   as the billboard/puff and world layers), the battle stats list, moves the effect textures
   `ofx_14p_03_01`, `bfx_151p_01`, `afx_101p_01`, `smoke`, `kemuri1`, `StopWall` to VRAM, resets
   the phase/flash/focus/end-zoom state, copies `g_colorBlack` into the display clear colour,
   seeds the RNG (fixed seed in netplay via `CoreRandResetSeed`/`CoreRandLoadVfpuState`,
   otherwise advanced by the low byte of the RTC microseconds), creates the HUD/talk task (id 0x6e,
   priority 0xfa), allocates the 0x80-byte stage package name buffer and three `IoLzsPackage`s.
   `focus` and `focusTarget` are cleared from the VFPU bank zero vector C720.
   Returns `self`. */

BtlMain *BtlMainTaskCtor(BtlMain *self)

{
  bool fromLow;
  GfxSpriteLayer *mem;
  GfxSpriteLayer *layer;
  IoLzsPackage *pkgMem;
  IoLzsPackage *pkg;
  char *name;
  s32 spins;
  s32 i;

  CoreTaskInit(&self->base);
  self->base.vtable = g_btlMainVtbl;
  BtlCameraCtor(&self->camera);
  g_lastScreenTaskId = 100;
  CoreBitsetSet(0x22, g_scriptGlobalBits);
  CoreBitsetClear(0x1f, g_scriptGlobalBits);
  CoreBitsetClear(0x23, g_scriptGlobalBits);
  g_btlItemSpawnEnabled = 0;
  if (SaveGetProfileFlag0() == 0) {
    SaveProfileSetWord(SaveGetProfile(), 0x13, 0);
    CorePowerSetCpuClock(333);
    g_actorCrystalSpawnRequest = 1;
  }
  else {
    NetCharaResetAllSync();
  }
  g_btlControlLockApplied = 1;
  self->field11 = 0;
  g_gfxDisplay->frameSkip = 0;
  CollisionInitQueryShapes();

  /* sprite layer 0: 2D */
  MemLock();
  fromLow = MemIsAllocFromLow();
  MemSetAllocFromLow(true);
  mem = MemAlloc(0x80, NULL, 0);
  MemSetAllocFromLow(fromLow);
  MemUnlock();
  layer = NULL;
  if (mem != NULL) {
    GfxSpriteLayerCtor(mem, 0);
    layer = mem;
  }
  self->spriteLayers[0] = layer;
  layer->sorted = 1;

  /* sprite layer 1: 3D billboards / puffs */
  MemLock();
  fromLow = MemIsAllocFromLow();
  MemSetAllocFromLow(true);
  mem = MemAlloc(0x80, NULL, 0);
  MemSetAllocFromLow(fromLow);
  MemUnlock();
  layer = NULL;
  if (mem != NULL) {
    GfxSpriteLayerCtor(mem, 1);
    layer = mem;
  }
  self->spriteLayers[1] = layer;
  layer->sorted = 1;
  g_billboardSpriteLayer = self->spriteLayers[1];
  g_puffSpriteLayer = self->spriteLayers[1];

  /* sprite layer 2: 3D world */
  MemLock();
  fromLow = MemIsAllocFromLow();
  MemSetAllocFromLow(true);
  mem = MemAlloc(0x80, NULL, 0);
  MemSetAllocFromLow(fromLow);
  MemUnlock();
  layer = NULL;
  if (mem != NULL) {
    GfxSpriteLayerCtor(mem, 1);
    layer = mem;
  }
  self->spriteLayers[2] = layer;
  g_worldSpriteLayer = layer;
  g_worldSpriteLayerCopy = self->spriteLayers[2];

  BtlStatsSystemInit(10);
  GfxTextureMoveToVram("ofx_14p_03_01");
  GfxTextureMoveToVram("bfx_151p_01");
  GfxTextureMoveToVram("afx_101p_01");
  GfxTextureMoveToVram("smoke");
  GfxTextureMoveToVram("kemuri1");
  GfxTextureMoveToVram("StopWall");

  self->phase = 0;
  self->drawPhase = 0;
  g_btlSceneFrameCount = 0;
  self->phaseStep = 0;
  self->savedPhase = 0;
  self->flashTarget = 0.0f;
  self->flashAlpha = 0.0f;
  self->flashDecay = 0.0f;
  self->float454 = 0.0f;
  /* C720 is the VFPU bank zero vector */
  for (i = 0; i < 4; i++) {
    self->focus[i] = 0.0f;
  }
  for (i = 0; i < 4; i++) {
    self->focusTarget[i] = 0.0f;
  }
  self->focusTarget[3] = 0.600000024f;
  self->focusFrames = 0;
  self->focusBlend = 0.0f;
  g_btlDepthTestEnabled = 1;
  g_btlBattleOutcome = 0;
  self->word53c = 0;
  g_btlBattleOver = 0;
  g_btlControlLockAll = 0;
  self->pad = g_padState;
  self->pad->dpadEmulatesStick = 0;
  self->pad->stickEmulatesDpad = 0;
  self->modelLists[1].tail = NULL;
  self->modelLists[1].head = NULL;
  self->modelLists[1].count = 0;
  self->modelLists[0].tail = NULL;
  self->modelLists[0].head = NULL;
  self->modelLists[0].count = 0;
  self->modelLists[2].tail = NULL;
  self->modelLists[2].head = NULL;
  self->modelLists[2].count = 0;
  self->fabListTail = NULL;
  self->fabList = NULL;
  self->fabListCount = 0;
  self->flag52c = 0;
  self->flag52d = 1;
  self->fieldFlags[0] = 0;
  self->fieldFlags[1] = 0;
  self->fieldFlags[2] = 0;
  self->fieldFlags[3] = 0;
  self->field10 = 0;
  g_btlHudHidden = 1;
  self->field12 = 0;
  self->field8 = 0;
  self->resultDemo = 0;
  self->talkStep = 0;
  self->bgmOverride = -1;
  self->endZoomFrames = 0;
  self->endZoomTarget = 0.0f;
  self->endZoomParam = 0.0f;
  self->endZoomProgress = 0.0f;
  self->endZoomStep = 0.0f;
  self->endFrame = 0;
  g_gfxDisplay->clearColor[0] = g_colorBlack.x;
  g_gfxDisplay->clearColor[1] = g_colorBlack.y;
  g_gfxDisplay->clearColor[2] = g_colorBlack.z;
  g_gfxDisplay->clearColor[3] = g_colorBlack.w;
  self->stageEffects = NULL;
  self->unitEffects = NULL;
  self->screenWave = NULL;
  self->talkUnit = NULL;
  self->dimColor[0] = 0.0f;
  self->dimColor[1] = 0.0f;
  self->dimColor[2] = 0.0f;
  self->dimColor[3] = 1.0f;

  if (SaveGetProfileFlag0() != 0) {
    CoreRandResetSeed();
    CoreRandLoadVfpuState();
  }
  else {
    spins = CoreRtcGetMicrosecond() & 0xff;
    for (i = 0; i < spins; i++) {
      CoreRandNext(0);
    }
  }
  self->netWaitFrames = 0;

  BtlMainInitLocalSlot(self);
  BtlNetSetBattleFlags(self, false);
  NetCharaResetAllSync();
  NetModeFlagClear();
  g_btlFrameCount = 0;
  g_btlControlLockApplied = 0;
  self->battleStarted = 0;
  self->flag559 = 0;
  self->keepRounds = 0;
  self->rematch = 0;
  BtlSetKindSetupMask(0);
  GfxSetMotionTimeScale(1.0f);
  CoreTaskCreate(0x6e, 0xfa);
  g_btlDepthTestEnabled = 1;
  for (i = 0; i < 3; i++) {
    self->endGradient[i] = 0;
  }
  self->endPulsePhase = 0.0f;

  MemLock();
  fromLow = MemIsAllocFromLow();
  MemSetAllocFromLow(true);
  name = MemAlloc(0x80, NULL, 0);
  MemSetAllocFromLow(fromLow);
  MemUnlock();
  self->stagePackName = name;
  self->standingTargetLimit = 0;
  self->busy = 0;
  self->skipAppearDemo = 0;
  self->roundRecorded = 0;
  self->winsCounted = 0;
  g_scriptGlobalVars[0xe] = 0;
  self->introTimer = 0;
  BtlAiEnableTargetLossHook();

  for (i = 0; i < 3; i++) {
    MemLock();
    fromLow = MemIsAllocFromLow();
    MemSetAllocFromLow(true);
    pkgMem = MemAlloc(0x44, NULL, 0);
    MemSetAllocFromLow(fromLow);
    MemUnlock();
    pkg = NULL;
    if (pkgMem != NULL) {
      IoLzsPackageCtor(pkgMem);
      pkg = pkgMem;
    }
    self->packages[i] = &pkg->base;
  }
  self->quitStep = 0;
  return self;
}
