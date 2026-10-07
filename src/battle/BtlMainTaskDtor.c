// bdc 0x088514a4 BtlMainTaskDtor
#include "bdc.h"

/* Destructor of the main battle-scene task (id 100): restores `g_btlMainVtbl`, waits for the GE,
   releases the Bakugan textures and moves the six preloaded effect textures back to main RAM
   (`GfxTextureMoveToMainRam`), kills the battle sub-tasks (all of ids 0x14a, 0x14b, 0x1e1, 0x1e0,
   0x6b, 0x69, 0x68, 0x65, 0x67, the first of id 0x1e2), clears the Bakugan list
   (`BtlSetBakuganList`), shuts the battle subsystems down, deletes the sprite layers, model
   chains, effect managers, mesh-object lists, colliders, motions and HP gauges, sets
   `g_btlDepthTestEnabled`, frees the stage pack name, flushes deferred deletions, stores the
   profile's word 0x31 as the stage number after a won rule-mode-1 battle, deletes the three
   packages, unmutes the sound listener, destroys the camera and chains to `CoreTaskDestroy`;
   frees the object when `flags & 1`. */

void BtlMainTaskDtor(BtlMain *self, u32 flags)
{
    const VtblEntry *dtor;
    GfxSpriteLayer *layer;
    GfxEffectMgr *effects;
    CoreNode *package;
    SaveProfile *profile;
    char *packName;
    s32 i;

    if (self == NULL) {
        return;
    }
    self->base.vtable = g_btlMainVtbl;
    GfxWaitGeIdle();
    BtlBakuganTexLoaderReleaseAll();
    GfxTextureMoveToMainRam("ofx_14p_03_01");
    GfxTextureMoveToMainRam("bfx_151p_01");
    GfxTextureMoveToMainRam("afx_101p_01");
    GfxTextureMoveToMainRam("smoke");
    GfxTextureMoveToMainRam("kemuri1");
    GfxTextureMoveToMainRam("StopWall");
    CoreTaskRemoveAllById(0x14a);
    CoreTaskRemoveAllById(0x14b);
    CoreTaskRemoveAllById(0x1e1);
    CoreTaskRemoveAllById(0x1e0);
    CoreTaskRemoveAllById(0x6b);
    CoreTaskRemoveAllById(0x69);
    CoreTaskRemoveAllById(0x68);
    CoreTaskRemoveAllById(0x65);
    CoreTaskRemoveAllById(0x67);
    CoreTaskRemoveById(0x1e2);
    BtlSetBakuganList(NULL);
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
    BtlLoadRequestsDestroyAll();

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
    GmoMotionFreeAll(GmoMotionMgrGet(), 0);
    CoreNodeDestroyChain(g_uiHpGaugeGroup.head);
    BtlSetBakuganList(NULL);
    GfxSetMotionTimeScale(1.0f);
    BtlStatsSystemShutdown();
    g_btlDepthTestEnabled = 1;

    packName = self->stagePackName;
    if (packName != NULL) {
        MemLock();
        MemFree(packName, NULL, 0);
        MemUnlock();
        self->stagePackName = NULL;
    }
    CoreObjectDeferDeleteFlush();

    if (g_scriptGlobalVars[8] == 1 && g_btlBattleOutcome == 1) {
        profile = SaveGetProfile();
        SaveProfileGetWord(profile, 0x2e);
        profile = SaveGetProfile();
        g_scriptGlobalVars[1] = SaveProfileGetWord(profile, 0x31);
    }

    for (i = 0; i < 3; i++) {
        package = self->packages[i];
        if (package != NULL) {
            dtor = &((const VtblEntry *)package->vtable)[1];
            ((void (*)(void *, s32))dtor->fn)((u8 *)package + dtor->delta, 3);
            self->packages[i] = NULL;
        }
    }

    if (SndHasListener()) {
        SndListenerSetMuted(SndGetListener(), 0);
    }
    BtlCameraDtor(&self->camera, 2);
    CoreTaskDestroy(&self->base, 0);
    if ((flags & 1) != 0) {
        MemLock();
        MemFree(self, NULL, 0);
        MemUnlock();
    }
}
