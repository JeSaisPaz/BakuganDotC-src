// bdc 0x08850800 BtlMainDrawTalk
#include "bdc.h"

/* Draw phase 4 of the battle main task (talk): with UI window 0 inactive just a white screen
   tint (sort key 0); otherwise, on sort key 110, the 2D state (`GfxPacketCall2DState`) and the
   talk window's two-band background gradient (`GfxPacketDrawGradientBands`, rows
   `g_btlTalkBackBandYs`, colours `UiTalkTask` `backColours`) plus `BtlMainDrawScene`. Then,
   on sort key 1050, the dim colour `dimColor` when its alpha is above 0. */

void BtlMainDrawTalk(BtlMain *self)
{
    void *packet;
    BtlHud *talk;
    float white[4] __attribute__((aligned(16)));

    if (UiGetWindowActive(0) == 0) {
        packet = GfxNewRenderPacket(0.0f);
        white[0] = 1.0f;
        white[1] = 1.0f;
        white[2] = 1.0f;
        white[3] = 1.0f;
        GfxPacketDrawScreenTint(packet, white, 1, NULL);
    } else {
        packet = GfxNewRenderPacket(110.0f);
        GfxPacketCall2DState(packet);
        talk = UiGetTalkTask();
        GfxPacketDrawGradientBands(packet, g_btlTalkBackBandYs, (const u32 *)talk->cutInColor, 2, 1);
        BtlMainDrawScene(self);
    }
    packet = GfxNewRenderPacket(1050.0f);
    if (!(self->dimColor[3] <= 0.0f)) {
        GfxPacketDrawScreenFlash(packet, (ScePspFVector4 *)self->dimColor);
    }
}
