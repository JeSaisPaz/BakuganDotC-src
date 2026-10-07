// bdc 0x089cf0e4 GfxDrawLoadMeter
#include "bdc.h"

/* Draws the debug CPU/GPU load bars of the frame: takes the topmost render packet
   (`GfxGetTopRenderPacket`), writes the 2D render state (`GfxPacketCall2DState`), then two
   rectangles `{x, y, w, h}` with `GfxPacketDrawRect` — x = 1 (CPU bar, height
   `cpuTime * fps * 0.0166667f`, colour `g_colorBlue`) and x = 7 (GPU bar, height
   `gpuTime * fps * 0.0166667f`, colour `g_colorRed`), each at y = 0 and 6 px wide — where `fps`
   is `GfxDisplayGetFps` (read again before each bar). Does nothing when there is no packet. */

void GfxDrawLoadMeter(GfxDisplay *display, s32 cpuTime, s32 gpuTime)
{
  RenderPacket *packet;
  s32 fps;
  float cpuRect[4];
  float gpuRect[4];

  packet = (RenderPacket *)GfxGetTopRenderPacket();
  if (packet != NULL) {
    GfxPacketCall2DState(packet);
    fps = GfxDisplayGetFps(display);
    cpuRect[1] = 0.0f;
    cpuRect[0] = 1.0f;
    cpuRect[2] = 6.0f;
    cpuRect[3] = (float)(cpuTime * fps) * 0.0166667f;
    GfxPacketDrawRect(packet, cpuRect, &g_colorBlue.x);
    fps = GfxDisplayGetFps(display);
    gpuRect[0] = 7.0f;
    gpuRect[1] = 0.0f;
    gpuRect[2] = 6.0f;
    gpuRect[3] = (float)(gpuTime * fps) * 0.0166667f;
    GfxPacketDrawRect(packet, gpuRect, &g_colorRed.x);
  }
}
