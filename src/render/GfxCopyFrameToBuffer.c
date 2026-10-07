// bdc 0x0882aac8 GfxCopyFrameToBuffer
#include "bdc.h"

/* When `g_blurCaptureActive` is set, copies 0x20000 bytes of the current frame buffer (VRAM base
   `sceGeEdramGetAddr` + `g_gfxFrameIndex × 0x88000`) into the buffer `g_blurCaptureBuffer`. Not
   called directly: `GfxPlayerBlurTaskStep` installs it as the display's signal hook
   (`g_gfxDisplay->signalHook`), so it runs once per displayed frame while the
   player blur task (`GfxPlayerBlurTaskCtor`) is active (`g_blurCaptureActive`). */

void GfxCopyFrameToBuffer(void)
{
  u8 *vram;

  if (g_blurCaptureActive != 0) {
    vram = sceGeEdramGetAddr();
    memcpy(g_blurCaptureBuffer, vram + g_gfxFrameIndex * 0x88000, 0x20000);
  }
}
