// bdc 0x089cefc8 GfxDisplayEndFrame
#include "bdc.h"

/* The per-frame render step: (1) if `showLoadMeter`, draws the previous frame's CPU/GPU bars
   (`GfxDrawLoadMeter`); (2) runs `frameHook`; (3) records the CPU time (`GfxDisplayGetCpuTime`)
   and waits for the GE (`GfxWaitGeIdle`); (4) records the GPU time (`GfxDisplayGetGpuTime`);
   (5) runs `SysUtilUpdateActive` when the system-utility manager exists, which has to happen
   between GE completion and the flip; (6) waits the vblanks and swaps (`GfxDisplayWaitVblank`);
   (7) stamps the new frame start (`GfxDisplayMarkFrameStart`); (8) closes, sorts and submits the
   frame's render packets (`GfxFlushRenderPackets`); (9) starts the next frame list
   (`GfxBeginFrame`). */

void GfxDisplayEndFrame(GfxDisplay *display)
{
  if (display->showLoadMeter != 0) {
    GfxDrawLoadMeter(display, display->cpuTime, display->gpuTime);
  }
  if (display->frameHook != 0) {
    display->frameHook();
  }
  display->cpuTime = GfxDisplayGetCpuTime(display);
  GfxWaitGeIdle();
  display->gpuTime = GfxDisplayGetGpuTime(display);
  if (SysUtilIsInit()) {
    SysUtilGetCell();
    SysUtilUpdateActive();
  }
  GfxDisplayWaitVblank(display);
  GfxDisplayMarkFrameStart(display);
  GfxFlushRenderPackets();
  GfxBeginFrame(display);
}
