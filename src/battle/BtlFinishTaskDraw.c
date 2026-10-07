// bdc 0x08848e88 BtlFinishTaskDraw
#include "bdc.h"

/* Draw of the end-of-battle cinematic task (`BtlFinishTask`): while `step` < 3, draws speed lines
   (`GfxPacketDrawSpeedLines`) in a sort-key-5 packet (`GfxNewRenderPacket`) centred on (240,
   136) with ellipse `flashT*16*16 + 60` × `flashT*9*16 + 33`, amount `0.6 - flashT*0.6` and colour
   `g_colorWhite`; then, unless `g_btlHudHidden` is set, draws mesh-object list 1
   (`GfxMeshObjDrawList1`) in a sort-key-0 packet. */

void BtlFinishTaskDraw(BtlFinishTask *self)
{
    void *packet;
    float t;

    if (self->step < 3) {
        packet = GfxNewRenderPacket(5.0f);
        t = self->flashT;
        GfxPacketDrawSpeedLines(240.0f, 136.0f, t * 16.0f * 16.0f + 60.0f, t * 9.0f * 16.0f + 33.0f,
                                0.6f - t * 0.6f, packet, &g_colorWhite.x);
    }
    if (g_btlHudHidden == 0) {
        GfxMeshObjDrawList1(GfxNewRenderPacket(0.0f));
    }
}
