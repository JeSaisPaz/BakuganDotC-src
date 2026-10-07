// bdc 0x08901918 BtlDemoDrawPlay
#include "bdc.h"

/* State 1 draw of the battle intro demo task (task id 0x65, `BtlDemoCtor`, update
   `BtlDemoUpdate` / draw `BtlDemoDraw` on the state `+0x65c`) (also used by the
   Bakugan-appearance demo task (task id 0x67, `BtlAppearDemoCreate`, `BtlAppearDemoCtor`,
   update `BtlAppearDemoUpdate`; derived from the battle intro demo)). Sort key 0: a chunk with
   the fog commands of `BtlStageGetFogParams` and the demo camera's light state holding the
   opaque map models (`mapModels`) and stage objects (`g_actorStageObjList`) via
   `BtlDemoDrawModels`; mesh list 1 at fog base 80 with the sprite depth test from
   `g_btlDepthTestEnabled`; the `effectLayer` pass (`BtlDemoDrawEffect`); the `flashColour`
   flash when its alpha is above 0.0001; a second chunk with the Bakugan (vtable slot `+0x44`, when
   its alpha `ambient[3]` is above 0), the depth-sorted `modelChain` (`BtlDemoDrawSortedModels`),
   the opaque `models`, the translucent map/stage/actor/model lists and the camera projection at
   fog base 50; debug collision primitives. Pad button `0x20` toggles `g_btlDemoDebugToggle`.
   Unless `BtlDemoIdIsVariant3` holds for `demoId`: the sprite layer, `particles1`, mesh list 0
   and `particles0` (models and sprites). Sort key 2: the white `flashLevel` tint (eased toward
   `flashAlpha` by 0.2 of the gap, step clamped to ±0.1, fading out with an accelerating
   `flashFadeSpeed`; drawn at 0.8 × level while above 0), the framebuffer copy, speed lines while
   `flashOn`, mesh list 2 and the top/bottom vignette bands. Sort key 1050: the `fadeColour`
   flash when its alpha is above 0. */

typedef void (*BtlDemoDrawHookFn)(void *self, u32 **list);

/* GCC 2.x virtual call of slot 8 (`+0x40` delta, `+0x44` function) of a CoreObject-based object. */
static void BtlDemoCallDrawHook(CoreObject *obj, u32 **list)
{
    const VtblEntry *entry = (const VtblEntry *)obj->vtable + 8;

    ((BtlDemoDrawHookFn)entry->fn)((u8 *)obj + entry->delta, list);
}

void BtlDemoDrawPlay(BtlDemo *demo)
{
    void *packet;
    BtlArenaFog *fog;
    BtlBakugan *bakugan;
    CoreObject *stageObjs;
    BtlDemoCam *camera;
    u32 *dl;
    float level;
    float alpha;
    float step;
    float tint[4] __attribute__((aligned(16)));
    union { float f; u32 u; } bits;

    packet = GfxNewRenderPacket(0.0f);
    dl = GfxPacketBeginChunk(packet);
    fog = (BtlArenaFog *)BtlStageGetFogParams();
    dl[0] = (fog->color & 0xffffff) | 0xcf000000;
    bits.f = fog->range;
    dl[1] = (bits.u >> 8) | 0xcd000000;
    bits.f = fog->scale;
    dl[2] = (bits.u >> 8) | 0xce000000;
    dl = dl + 3;
    camera = &demo->cam;
    dl = GfxDlWriteLightState(dl, &camera->base, 1);
    dl = BtlDemoDrawModels(demo, dl, (void **)demo->mapModels, false);
    dl = BtlDemoDrawModels(demo, dl, (void **)g_actorStageObjList, false);
    GfxPacketEndChunk(packet, dl);

    g_gfxSpriteDepthTest = g_btlDepthTestEnabled;
    g_gfxMeshObjDepthTest = g_btlDepthTestEnabled;
    camera->base.fogBase = 80.0f;
    g_gfxMeshObjDepthTest = 0;
    GfxMeshObjDrawList1(packet);
    g_gfxMeshObjDepthTest = g_btlDepthTestEnabled;
    if (demo->effectLayer != NULL) {
        BtlDemoDrawEffect(demo, packet, demo->effectLayer);
    }
    camera->base.fogBase = 0.0f;
    if (!(demo->flashColour[3] <= 0.0001f)) {
        GfxPacketDrawScreenFlash(packet, (const ScePspFVector4 *)demo->flashColour);
    }

    dl = GfxPacketBeginChunk(packet);
    dl = GfxDlWriteLightState(dl, &camera->base, 1);
    bakugan = demo->bakugan;
    if (bakugan != NULL && !(bakugan->base.ambient[3] <= 0.0f)) {
        BtlDemoCallDrawHook(&bakugan->base.base, &dl);
    }
    dl = BtlDemoDrawSortedModels(&demo->base, dl, (void **)&demo->modelChain);
    dl = BtlDemoDrawModels(demo, dl, (void **)demo->models, false);
    GfxModelListDrawTranslucent(&dl, demo->mapModels->head, true);
    stageObjs = NULL;
    if (g_actorStageObjList != NULL) {
        stageObjs = g_actorStageObjList->head;
    }
    GfxModelListDrawTranslucent(&dl, stageObjs, true);
    GfxModelListDrawTranslucent(&dl, demo->actorList.head, true);
    GfxModelListDrawTranslucent(&dl, demo->models->head, true);
    camera->base.fogBase = 50.0f;
    dl = GfxCameraDlWrite(&camera->base, dl, 4);
    GfxPacketEndChunk(packet, dl);
    CollisionDebugPrimsDraw(packet);

    if ((demo->pad->pressed & 0x20) != 0) {
        g_btlDemoDebugToggle = (g_btlDemoDebugToggle == 0);
    }
    camera->base.fogBase = 0.0f;
    if (!BtlDemoIdIsVariant3(demo->demoId)) {
        GfxSpriteLayerDrawWorld(demo->spriteLayer, packet, camera, NULL);
        if (demo->particles1 != NULL) {
            GfxSpriteLayerDrawWorld(&demo->particles1->base, packet, camera, NULL);
        }
        GfxMeshObjDrawList0(packet);
        g_gfxSpriteDepthTest = 0;
        g_gfxMeshObjDepthTest = 0;
        if (demo->particles0 != NULL) {
            GfxEffectMgrDrawModels(demo->particles0, packet, camera);
            GfxSpriteLayerDrawWorld(&demo->particles0->base, packet, camera, NULL);
        }
    }

    packet = GfxNewRenderPacket(2.0f);
    level = demo->flashLevel;
    alpha = demo->flashAlpha;
    if (level != 0.0f || alpha != 0.0f) {
        step = (alpha - level) * 0.2f;
        if (!(step <= 0.1f)) {
            step = 0.1f;
        } else if (step < -0.1f) {
            step = -0.1f;
        }
        if (level <= alpha) {
            demo->flashFadeSpeed = 0.0f;
            level = level + step;
        } else {
            demo->flashFadeSpeed = demo->flashFadeSpeed - 0.002f;
            level = level + demo->flashFadeSpeed;
        }
        demo->flashLevel = level;
        if (level < 0.0f) {
            demo->flashLevel = 0.0f;
            level = 0.0f;
        }
        if (!(level <= 0.0f)) {
            tint[0] = 1.0f;
            tint[1] = 1.0f;
            tint[2] = 1.0f;
            tint[3] = level * 0.8f;
            GfxPacketDrawScreenTint(packet, tint, 1, NULL);
        }
    }
    GfxPacketCopyFramebuffer(packet, NULL);
    if (demo->flashOn != 0) {
        GfxPacketDrawSpeedLines(240.0f, 136.0f, 120.0f, 68.0f, 0.5f, packet, &g_colorWhite.x);
    }
    GfxMeshObjDrawList2(packet);
    g_gfxSpriteDepthTest = 1;
    g_gfxMeshObjDepthTest = 1;
    GfxPacketCall2DState(packet);
    GfxPacketDrawGradientBands(packet, g_btlDemoVignetteTopYs, g_btlDemoVignetteTopColours, 1, 2);
    GfxPacketDrawGradientBands(packet, g_btlDemoVignetteBottomYs, g_btlDemoVignetteBottomColours, 1, 3);

    packet = GfxNewRenderPacket(1050.0f);
    if (!(demo->fadeColour.w <= 0.0f)) {
        GfxPacketDrawScreenFlash(packet, &demo->fadeColour);
    }
}
