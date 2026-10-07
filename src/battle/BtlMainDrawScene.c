// bdc 0x08850220 BtlMainDrawScene
#include "bdc.h"

/* Draw handler of the battle main task (task id 100, `BtlMainTaskCtor`, vtable `0x08af18f4`; the
   task `BtlGetCameraTask` returns) for draw phases 1 and 2 (table `0x08a66494`, `+0x444`), also
   called by `BtlMainDrawTalk`. Sort key 0: no-fog mesh objects (`GfxMeshObjDrawList4`), then a
   chunk with the fog commands of `g_gfxFogParams` and the light state of `g_gfxActiveCamera`
   holding either the opaque model list 1 plus the stage objects (`g_actorStageObjList`) when
   `g_btlHudHidden` is set, or the targeted stage objects (`ActorStageObjNotifyTargeted`);
   then the `focus` colour flash when its alpha is above 0.0001. Sort key 1, with the sprite/mesh
   depth tests taken from `g_btlDepthTestEnabled`: (HUD hidden) mesh lists 3 and 1 and sprite
   layer 2 at fog base 80; a chunk with either the player's unit and its target drawn through their
   vtable slot `+0x44` (phase 5, HUD shown) or the opaque model list 0 plus `BtlDrawModelList` on
   list 2, then the translucent lists 0 and 2 and the camera projection at fog base 50; debug
   collision primitives; sprite layer 1 and the stage/unit effect managers; mesh list 0; (HUD hidden)
   the translucent model list 1 and stage objects. Then the 2D overlays: the talk task's two-band
   overlay (key 110) while the player's unit is respawning and the talk window exists, else the
   top/bottom vignette bands (key 1.9) outside stages 0/13; the `endGradient` bands (key 150) while
   `field11` is set; the flash/copy pass (`BtlMainDrawFlashAndCopy`) with the HP gauges, the fab
   list, sprite layer 0 (key 3) and the `dimColor` flash (key 1050) when its alpha is above 0. Pad
   button `0x20` toggles `g_btlDebugToggle`. */

typedef void (*BtlDrawHookFn)(void *self, u32 **list);

/* GCC 2.x virtual call of slot 8 (`+0x40` delta, `+0x44` function) of a CoreObject-based object. */
static void BtlMainCallDrawHook(CoreObject *obj, u32 **list)
{
    const VtblEntry *entry = (const VtblEntry *)obj->vtable + 8;

    ((BtlDrawHookFn)entry->fn)((u8 *)obj + entry->delta, list);
}

void BtlMainDrawScene(BtlMain *self)
{
    BtlArenaFog *fog;
    void *packet;
    void *scenePacket;
    BtlBakugan *player;
    CoreObject *target;
    CoreObject *stageObjs;
    UiTalkTask *talk;
    u32 *dl;
    union { float f; u32 u; } bits;

    packet = GfxNewRenderPacket(0.0f);
    GfxMeshObjDrawList4(packet);
    dl = GfxPacketBeginChunk(packet);
    fog = g_gfxFogParams;
    dl[0] = (fog->color & 0xffffff) | 0xcf000000;
    bits.f = fog->range;
    dl[1] = (bits.u >> 8) | 0xcd000000;
    bits.f = fog->scale;
    dl[2] = (bits.u >> 8) | 0xce000000;
    dl = dl + 3;
    dl = GfxDlWriteLightState(dl, g_gfxActiveCamera, 1);
    *dl = 0x19000001;
    dl = dl + 1;
    if (g_btlHudHidden == 0) {
        ActorStageObjNotifyTargeted(&dl);
    } else {
        GfxModelListDrawOpaque(&dl, self->modelLists[1].head, true);
        stageObjs = NULL;
        if (g_actorStageObjList != NULL) {
            stageObjs = g_actorStageObjList->head;
        }
        GfxModelListDrawOpaque(&dl, stageObjs, true);
    }
    GfxPacketEndChunk(packet, dl);
    if (!(self->focus[3] <= 0.0001f)) {
        GfxPacketDrawScreenFlash(packet, (const ScePspFVector4 *)self->focus);
    }

    scenePacket = GfxNewRenderPacket(1.0f);
    g_gfxSpriteDepthTest = g_btlDepthTestEnabled;
    g_gfxMeshObjDepthTest = g_btlDepthTestEnabled;
    if (g_btlHudHidden != 0) {
        GfxMeshObjDrawList3(scenePacket);
        g_gfxActiveCamera->fogBase = 80.0f;
        GfxMeshObjDrawList1(scenePacket);
        GfxSpriteLayerDrawWorld(self->spriteLayers[2], scenePacket, g_gfxActiveCamera, NULL);
        g_gfxActiveCamera->fogBase = 0.0f;
    }
    dl = GfxPacketBeginChunk(scenePacket);
    dl = GfxDlWriteLightState(dl, g_gfxActiveCamera, 1);
    *dl = 0x19000001;
    dl = dl + 1;
    if (g_btlHudHidden == 0 && self->phase == 5) {
        player = BtlGetPlayerBakugan();
        if (player != NULL) {
            BtlMainCallDrawHook(&player->base.base, &dl);
            target = BtlBakuganGetTarget(player);
            if (target != NULL) {
                BtlMainCallDrawHook(target, &dl);
            }
        }
    } else {
        GfxModelListDrawOpaque(&dl, self->modelLists[0].head, true);
        dl = BtlDrawModelList(self, dl, (void **)&self->modelLists[2]);
    }
    GfxModelListDrawTranslucent(&dl, self->modelLists[0].head, true);
    GfxModelListDrawTranslucent(&dl, self->modelLists[2].head, true);
    g_gfxActiveCamera->fogBase = 50.0f;
    dl = GfxCameraDlWrite(g_gfxActiveCamera, dl, 4);
    GfxPacketEndChunk(scenePacket, dl);
    CollisionDebugPrimsDraw(scenePacket);
    if ((self->pad->pressed & 0x20) != 0) {
        g_btlDebugToggle = (g_btlDebugToggle == 0);
    }
    g_gfxActiveCamera->fogBase = 0.0f;
    GfxSpriteLayerDrawWorld(self->spriteLayers[1], scenePacket, g_gfxActiveCamera, NULL);
    GfxEffectMgrDrawModels(self->stageEffects, scenePacket, g_gfxActiveCamera);
    GfxSpriteLayerDrawWorld(self->stageEffects, scenePacket, g_gfxActiveCamera, NULL);
    GfxEffectMgrDrawModels(self->unitEffects, scenePacket, g_gfxActiveCamera);
    GfxSpriteLayerDrawWorld(self->unitEffects, scenePacket, g_gfxActiveCamera, NULL);
    GfxMeshObjDrawList0(scenePacket);
    g_gfxSpriteDepthTest = 1;
    g_gfxMeshObjDepthTest = 1;
    if (g_btlHudHidden != 0) {
        dl = GfxPacketBeginChunk(scenePacket);
        dl = GfxDlWriteLightState(dl, g_gfxActiveCamera, 1);
        *dl = 0x19000001;
        dl = dl + 1;
        GfxModelListDrawTranslucent(&dl, self->modelLists[1].head, true);
        stageObjs = NULL;
        if (g_actorStageObjList != NULL) {
            stageObjs = g_actorStageObjList->head;
        }
        GfxModelListDrawTranslucent(&dl, stageObjs, true);
        *dl = 0x19000000;
        dl = dl + 1;
        GfxPacketEndChunk(scenePacket, dl);
    }

    player = BtlGetPlayerBakugan();
    if (player != NULL) {
        if (player->respawnCountdown != 0) {
            if (UiTalkTaskExists() != 0) {
                packet = GfxNewRenderPacket(110.0f);
                GfxPacketCall2DState(packet);
                talk = (UiTalkTask *)UiGetTalkTask();
                GfxPacketDrawGradientBands(packet, g_btlTalkGradientYs, talk->overlayColours, 2, 1);
            }
        } else if (GameStageIs0Or13() == 0) {
            packet = GfxNewRenderPacket(1.9f);
            GfxPacketCall2DState(packet);
            GfxPacketDrawGradientBands(packet, g_btlVignetteTopYs, g_btlVignetteTopColours, 1, 2);
            GfxPacketDrawGradientBands(packet, g_btlVignetteBottomYs, g_btlVignetteBottomColours, 1, 3);
        }
    }
    if (self->field11 != 0) {
        packet = GfxNewRenderPacket(150.0f);
        GfxPacketCall2DState(packet);
        GfxPacketDrawGradientBands(packet, g_btlEndGradientYs, self->endGradient, 2, 1);
    }
    packet = BtlMainDrawFlashAndCopy(self);
    UiHpGaugeDraw(packet);
    GfxFabListDraw(&self->fabList);
    packet = GfxNewRenderPacket(3.0f);
    GfxSpriteLayerDraw(self->spriteLayers[0], packet);
    packet = GfxNewRenderPacket(1050.0f);
    if (!(self->dimColor[3] <= 0.0f)) {
        GfxPacketDrawScreenFlash(packet, (const ScePspFVector4 *)self->dimColor);
    }
}
