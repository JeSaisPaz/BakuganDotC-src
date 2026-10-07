// bdc 0x0884ffc4 BtlMainDrawExit
#include "bdc.h"

/* Draw phase 3 of the battle main task (exit): on a render packet at depth 499 draws a white
   full-screen tint (mode 2), then the `"back_04"` texture over the full 480x272 screen (the rect
   doubles as the UV rect) in white with the alpha of the pause task's `hintAlpha` (task 0x19a;
   alpha 0 when no pause task exists). */

void BtlMainDrawExit(BtlMain *self)
{
    void *packet;
    void *texture;
    UiPause *pause;
    float alpha;
    ScePspFVector4 colour __attribute__((aligned(16)));
    float rect[4] __attribute__((aligned(16)));

    (void)self;
    packet = GfxNewRenderPacket(499.0f);
    colour.x = 1.0f;
    colour.y = 1.0f;
    colour.z = 1.0f;
    colour.w = 1.0f;
    GfxPacketDrawScreenTint(packet, &colour.x, 2, NULL);
    alpha = 0.0f;
    rect[0] = 0.0f;
    rect[1] = 0.0f;
    rect[2] = 480.0f;
    rect[3] = 272.0f;
    pause = CoreTaskFind(0x19a);
    if (pause != NULL) {
        alpha = pause->hintAlpha;
    }
    texture = GfxFindTexture("back_04");
    colour.x = 1.0f;
    colour.y = 1.0f;
    colour.z = 1.0f;
    colour.w = alpha;
    GfxPacketDrawTexturedRect(packet, &g_gfxVecZero.x, rect, (const ScePspFVector4 *)rect, texture,
                              &colour);
}
