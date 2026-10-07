// bdc 0x088500bc BtlMainDrawFlashAndCopy
#include "bdc.h"

/* Draws the battle's white hit flash on layer 2 and returns the packet. While `flashAlpha` or
   `flashTarget` is non-zero, `flashAlpha` moves toward `flashTarget` by 0.2 x the gap clamped to
   [-0.1, 0.1] (rising or holding: `flashDecay` = 0), or, above the target, by `flashDecay`, which
   is first lowered by 0.002 per frame; it is clamped at 0, and while above 0 a white screen tint
   of alpha 0.8 x `flashAlpha` is drawn (`GfxPacketDrawScreenTint`). Then it copies the
   framebuffer (`GfxPacketCopyFramebuffer`) and draws `GfxMeshObjDrawList2` into the same
   packet. Used by `BtlMainDrawScene` and `BtlMainDrawCutIn`. */
void *BtlMainDrawFlashAndCopy(BtlMain *self)
{
    void *packet;
    float alpha;
    float target;
    float step;
    float tint[4] __attribute__((aligned(16))); /* GfxPacketDrawScreenTint reads it with lv.q */

    packet = GfxNewRenderPacket(2.0f);
    alpha = self->flashAlpha;
    target = self->flashTarget;
    if (alpha != 0.0f || target != 0.0f) {
        step = (target - alpha) * 0.200000003f;
        if (!(step <= 0.100000001f)) {
            step = 0.100000001f;
        } else if (step < -0.100000001f) {
            step = -0.100000001f;
        }
        if (alpha <= target) {
            self->flashDecay = 0.0f;
        } else {
            step = self->flashDecay - 0.00200000009f;
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
    GfxPacketCopyFramebuffer(packet, NULL);
    GfxMeshObjDrawList2(packet);
    return packet;
}
