// bdc 0x0884fdf4 BtlMainDrawLoad
#include "bdc.h"

/* Draw phase 0 of the battle main task (load), one render packet per layer: sprite layer 0
   (`GfxSpriteLayerDraw`, key 1), the screen wave (`BtlScreenWaveDraw`, key 1.5, when set), the
   white hit flash (key 1.8: `flashAlpha` steps toward `flashTarget` by (target - alpha) * 0.3
   clamped to ±0.1, or while above the target by `flashDecay`, which then drops by 0.004 per frame
   and otherwise resets to 0; clamped at 0 and drawn with alpha 0.8 × flashAlpha while above 0,
   `GfxPacketDrawScreenTint`), the framebuffer copy (key 2, `GfxPacketCopyFramebuffer`), the
   scene dim colour (key 1050, `GfxPacketDrawScreenFlash`, only while its alpha > 0) and finally
   the `.fab` list (`GfxFabListDraw`). */
void BtlMainDrawLoad(BtlMain *self)
{
    void *packet;
    float alpha;
    float target;
    float step;
    float tint[4] __attribute__((aligned(16))); /* GfxPacketDrawScreenTint reads it with lv.q */

    packet = GfxNewRenderPacket(1.0f);
    GfxSpriteLayerDraw(self->spriteLayers[0], packet);
    packet = GfxNewRenderPacket(1.5f);
    if (self->screenWave != NULL) {
        BtlScreenWaveDraw(self->screenWave, packet);
    }
    packet = GfxNewRenderPacket(1.79999995f);
    alpha = self->flashAlpha;
    target = self->flashTarget;
    if (alpha != 0.0f || target != 0.0f) {
        step = (target - alpha) * 0.300000012f;
        if (!(step <= 0.100000001f)) {
            step = 0.100000001f;
        } else if (step < -0.100000001f) {
            step = -0.100000001f;
        }
        if (alpha <= target) {
            self->flashDecay = 0.0f;
        } else {
            step = self->flashDecay - 0.00400000019f;
            self->flashDecay = step;
        }
        alpha = alpha + step;
        self->flashAlpha = alpha;
        if (alpha < 0.0f) {
            self->flashAlpha = 0.0f;
            alpha = 0.0f;
        }
        if (!(alpha <= 0.0f)) {
            tint[0] = 1.0f;
            tint[1] = 1.0f;
            tint[2] = 1.0f;
            tint[3] = alpha * 0.800000012f;
            GfxPacketDrawScreenTint(packet, tint, 1, NULL);
        }
    }
    packet = GfxNewRenderPacket(2.0f);
    GfxPacketCopyFramebuffer(packet, NULL);
    packet = GfxNewRenderPacket(1050.0f);
    if (!(self->dimColor[3] <= 0.0f)) {
        GfxPacketDrawScreenFlash(packet, (const ScePspFVector4 *)self->dimColor);
    }
    GfxFabListDraw(&self->fabList);
}
