// bdc 0x088508f4 BtlMainDrawCutIn
#include "bdc.h"

/* Draw phase 5 of the battle main task (cut-in). Packet 0: the fog commands from g_gfxFogParams,
   the light state and the targeted stage objects. Packet 1: the light state and only the local
   player's Bakugan and its current target (virtual draw, slot 8), then the world sprite layer 1
   and the stage effect set. Then the hit flash / framebuffer copy (BtlMainDrawFlashAndCopy), the
   HP gauges into the packet it returns, the .fab list, and the 2D sprite layer 0 in packet 3. */

/* GE command: the float's raw bits shifted right by 8 (24-bit float) under `cmd`. */
static u32 GeFloat24(u32 cmd, float value)
{
    union {
        float f;
        u32 u;
    } bits;

    bits.f = value;
    return (bits.u >> 8) | cmd;
}

/* Calls the draw virtual (slot 8) of a model with the display-list cursor. */
static void DrawModel(GfxModel *model, u32 **list)
{
    const VtblEntry *entry = &((const VtblEntry *)model->base.vtable)[8];

    ((void (*)(void *, u32 **))entry->fn)((u8 *)model + entry->delta, list);
}

void BtlMainDrawCutIn(BtlMain *self)
{
    void *packet;
    u32 *list;
    BtlBakugan *bakugan;
    GfxModel *target;

    packet = GfxNewRenderPacket(0.0f);
    list = GfxPacketBeginChunk(packet);
    list[0] = (g_gfxFogParams->color & 0xffffff) | 0xcf000000;
    list[1] = GeFloat24(0xcd000000, g_gfxFogParams->range);
    list[2] = GeFloat24(0xce000000, g_gfxFogParams->scale);
    list = GfxDlWriteLightState(list + 3, g_gfxActiveCamera, 1);
    *list = 0x19000001;
    list++;
    ActorStageObjNotifyTargeted(&list);
    GfxPacketEndChunk(packet, list);

    packet = GfxNewRenderPacket(1.0f);
    list = GfxPacketBeginChunk(packet);
    list = GfxDlWriteLightState(list, g_gfxActiveCamera, 1);
    *list = 0x19000001;
    list++;
    bakugan = BtlGetPlayerBakugan();
    if (bakugan != NULL) {
        DrawModel(&bakugan->base, &list);
        target = BtlBakuganGetTarget(bakugan);
        if (target != NULL) {
            DrawModel(target, &list);
        }
    }
    GfxPacketEndChunk(packet, list);
    GfxSpriteLayerDrawWorld(self->spriteLayers[1], packet, g_gfxActiveCamera, NULL);
    GfxEffectMgrDrawModels(self->stageEffects, packet, g_gfxActiveCamera);
    GfxSpriteLayerDrawWorld(self->stageEffects, packet, g_gfxActiveCamera, NULL);

    UiHpGaugeDraw(BtlMainDrawFlashAndCopy(self));
    GfxFabListDraw(&self->fabList);
    GfxSpriteLayerDraw(self->spriteLayers[0], GfxNewRenderPacket(3.0f));
}
