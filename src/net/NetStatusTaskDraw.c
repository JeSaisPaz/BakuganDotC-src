// bdc 0x08943dec NetStatusTaskDraw
#include "bdc.h"

/* Draw method of the netplay status overlay (id 2002): unless the alpha is <= 0, draws the text
   box (`UiTextBoxDraw`, skipped when there is none) and, in a render packet one depth unit
   below it, a full-screen green tint {0, 0.8, 0, alpha * 0.75} (`GfxPacketDrawScreenTint`)
   followed by `GfxPacketCopyFramebuffer`. The box depth is read even when `box` is NULL. */

void NetStatusTaskDraw(NetStatusTask *self)
{
    void *packet;
    float tint[4] __attribute__((aligned(16)));

    if (!(self->alpha <= 0.0f)) {
        if (self->box != NULL) {
            UiTextBoxDraw(self->box);
        }
        packet = GfxNewRenderPacket(self->box->packetDepth - 1.0f);
        tint[3] = self->alpha * 0.75f;
        tint[0] = 0.0f;
        tint[1] = 0.8f;
        tint[2] = 0.0f;
        GfxPacketDrawScreenTint(packet, tint, 1, NULL);
        GfxPacketCopyFramebuffer(packet, NULL);
    }
}
