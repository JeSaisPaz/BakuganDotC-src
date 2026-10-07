// bdc 0x08852c64 BtlMainTeardown
#include "bdc.h"

/* Tears the current battle down and resets the battle main task for a fresh start (called by
   `BtlMainPhaseFinish` at its last step). Waits for the GE (`GfxWaitGeIdle`), removes every task
   of ids 0x14a, 0x14b, 0x1e1, 0x1e0, 0x6b and the first of ids 0x6e, 0x6c, 0x69, 0x1e2, sets script
   flag 0x22 and clears flag 0x1f, clears the Bakugan list, destroys the sound emitters, the event
   script, the attacks and the items, deletes the sprite layers, model chains, stage objects, fab
   list, effect models and both effect managers (publishing the cleared pointers), the six
   mesh-object lists, the colliders and the HP gauges, restores the motion time scale, shuts the
   stats system down and flushes deferred deletions. It then rebuilds the three sprite layers from
   the low heap (2D sorted, world sorted, world; published to the billboard/puff and world layer
   globals), re-creates the stats system (capacity 10), resets the task's fields, the fader (black
   to clear over 1 frame), the random seed in net play, the local slot and the net state, creates
   task 0x6e (priority 0xfa), restarts at phase 0 step 0xf and unmutes the sound listener.
   `focus` and `focusTarget` are zeroed from the VFPU bank constant C720 = (0, 0, 0, 0). */
void BtlMainTeardown(BtlMain *self)
{
    const VtblEntry *dtor;
    GfxSpriteLayer *layer;
    GfxSpriteLayer *mem;
    GfxEffectMgr *effects;
    GfxFader *fader;
    PadState *pad;
    bool fromLow;
    s32 i;

    GfxWaitGeIdle();
    CoreTaskRemoveAllById(0x14a);
    CoreTaskRemoveAllById(0x14b);
    CoreTaskRemoveAllById(0x1e1);
    CoreTaskRemoveAllById(0x1e0);
    CoreTaskRemoveAllById(0x6b);
    CoreTaskRemoveById(0x6e);
    CoreTaskRemoveById(0x6c);
    CoreTaskRemoveById(0x69);
    CoreTaskRemoveById(0x1e2);
    CoreBitsetSet(0x22, g_scriptGlobalBits);
    CoreBitsetClear(0x1f, g_scriptGlobalBits);
    BtlSetBakuganList(NULL);
    if (SndHasListener()) {
        SndEmitterDestroyAll(SndGetListener());
    }
    BtlStageDestroyEventScript();
    BtlAttackSystemShutdown();
    BtlItemListClear();

    layer = self->spriteLayers[1];
    if (layer != NULL) {
        dtor = &layer->vtbl[1];
        ((void (*)(void *, s32))dtor->fn)((u8 *)layer + dtor->delta, 3);
        self->spriteLayers[1] = NULL;
    }
    layer = self->spriteLayers[0];
    if (layer != NULL) {
        dtor = &layer->vtbl[1];
        ((void (*)(void *, s32))dtor->fn)((u8 *)layer + dtor->delta, 3);
        self->spriteLayers[0] = NULL;
    }
    GfxModelChainDeleteAll(self->modelLists[0].head);
    GfxModelChainDeleteAll(self->modelLists[1].head);
    GfxModelChainDeleteAll(self->modelLists[2].head);
    ActorStageObjSystemShutdown();
    CoreObjectListDeleteAll((CoreObjectList *)&self->fabList);
    GfxEffectModelsFree();

    effects = (GfxEffectMgr *)self->stageEffects;
    if (effects != NULL) {
        dtor = &effects->base.vtbl[1];
        ((void (*)(void *, s32))dtor->fn)((u8 *)effects + dtor->delta, 3);
        self->stageEffects = NULL;
    }
    effects = (GfxEffectMgr *)self->unitEffects;
    if (effects != NULL) {
        dtor = &effects->base.vtbl[1];
        ((void (*)(void *, s32))dtor->fn)((u8 *)effects + dtor->delta, 3);
        self->unitEffects = NULL;
    }
    layer = self->spriteLayers[2];
    if (layer != NULL) {
        dtor = &layer->vtbl[1];
        ((void (*)(void *, s32))dtor->fn)((u8 *)layer + dtor->delta, 3);
        self->spriteLayers[2] = NULL;
    }
    g_worldEffectMgr = self->stageEffects;
    g_btlUnitEffectMgr = self->unitEffects;

    CoreObjectChainDeleteAll(g_gfxMeshObjList0.head);
    CoreObjectChainDeleteAll(g_gfxMeshObjList1.head);
    CoreObjectChainDeleteAll(g_gfxMeshObjList2.head);
    CoreObjectChainDeleteAll(g_gfxMeshObjList3.head);
    CoreObjectChainDeleteAll(g_gfxMeshObjList4.head);
    CoreObjectChainDeleteAll(g_gfxMeshObjList5.head);
    CollisionDeleteAllColliders();
    CoreNodeDestroyChain(g_uiHpGaugeGroup.head);
    BtlSetBakuganList(NULL);
    GfxSetMotionTimeScale(1.0f);
    BtlStatsSystemShutdown();
    CoreObjectDeferDeleteFlush();

    /* The layer pointer is used unchecked: a failed allocation writes `sorted` through NULL. */
    MemLock();
    fromLow = MemIsAllocFromLow();
    MemSetAllocFromLow(true);
    mem = MemAlloc(sizeof(GfxSpriteLayer), NULL, 0);
    MemSetAllocFromLow(fromLow);
    MemUnlock();
    layer = NULL;
    if (mem != NULL) {
        GfxSpriteLayerCtor(mem, 0);
        layer = mem;
    }
    self->spriteLayers[0] = layer;
    layer->sorted = 1;

    MemLock();
    fromLow = MemIsAllocFromLow();
    MemSetAllocFromLow(true);
    mem = MemAlloc(sizeof(GfxSpriteLayer), NULL, 0);
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

    MemLock();
    fromLow = MemIsAllocFromLow();
    MemSetAllocFromLow(true);
    mem = MemAlloc(sizeof(GfxSpriteLayer), NULL, 0);
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
    self->phase = 0;
    self->drawPhase = 0;
    g_btlSceneFrameCount = 0;
    self->phaseStep = 0;
    self->savedPhase = 0;
    self->flashTarget = 0.0f;
    self->flashAlpha = 0.0f;
    self->flashDecay = 0.0f;
    self->float454 = 0.0f;
    /* sv.q of the bank constant C720 = (0, 0, 0, 0). */
    self->focus[0] = 0.0f;
    self->focus[1] = 0.0f;
    self->focus[2] = 0.0f;
    self->focus[3] = 0.0f;
    self->focusTarget[0] = 0.0f;
    self->focusTarget[1] = 0.0f;
    self->focusTarget[2] = 0.0f;
    self->focusTarget[3] = 0.0f;
    self->focusTarget[3] = 0.6f;
    self->focusFrames = 0;
    self->focusBlend = 0.0f;
    g_btlDepthTestEnabled = 1;
    g_btlBattleOutcome = 0;
    self->word53c = 0;
    g_btlBattleOver = 0;
    g_btlControlLockAll = 0;
    pad = g_padState;
    self->pad = pad;
    pad->dpadEmulatesStick = 0;
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
    self->resultDemo = 0;
    self->talkStep = 0;
    self->bgmOverride = -1;
    self->endZoomFrames = 0;
    self->endZoomTarget = 0.0f;
    self->endZoomParam = 0.0f;
    self->endZoomProgress = 0.0f;
    self->endZoomStep = 0.0f;
    self->endFrame = 0;
    self->stageEffects = NULL;
    self->unitEffects = NULL;
    self->screenWave = NULL;
    self->talkUnit = NULL;
    self->dimColor[0] = 0.0f;
    self->dimColor[1] = 0.0f;
    self->dimColor[2] = 0.0f;
    self->dimColor[3] = 1.0f;

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
    GfxFaderStart(GfxGetActiveFader(), 1);

    if (SaveGetProfileFlag0()) {
        CoreRandResetSeed();
        CoreRandLoadVfpuState();
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
    CoreTaskCreate(0x6e, 0xfa);
    self->phase = 0;
    self->drawPhase = 0;
    self->phaseStep = 0xf;
    g_btlDepthTestEnabled = 1;
    g_btlCameraDefaultMode = 1;
    for (i = 0; i < 3; i++) {
        self->endGradient[i] = 0;
    }
    self->endPulsePhase = 0.0f;
    self->busy = 0;
    g_scriptGlobalVars[14] = 0;
    g_scriptGlobalVars[3] = 0;
    self->introTimer = 0;
    self->field8 = 0;
    self->field11 = 0;
    if (SndHasListener()) {
        SndListenerSetMuted(SndGetListener(), 0);
    }
}
