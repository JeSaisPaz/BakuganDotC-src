// bdc 0x089f2460 GfxRenderShutdown
#include "bdc.h"

/* Releases the render system's buffers: the display-chunk node pool (`g_renderChunkPool`), the render
   packet pool `g_renderPacketPool` and the display-list buffer (`g_renderListBuf`), then
   `GfxRenderPacketsReset` and the destructor call of the 0x2a0-byte scene object `g_gfxScreenCamera`
   (`GfxScreenCameraDestroy`). */

void GfxRenderShutdown(void)

{
  if (g_renderChunkPool != (void *)0x0) {
    MemLock();
    MemFree(g_renderChunkPool,(char *)0x0,0);
    MemUnlock();
    g_renderChunkPool = (void *)0x0;
  }
  if (g_renderPacketPool != (void *)0x0) {
    MemLock();
    MemFree(g_renderPacketPool,(char *)0x0,0);
    MemUnlock();
    g_renderPacketPool = (void *)0x0;
  }
  if (g_renderListBuf != (void *)0x0) {
    MemLock();
    MemFree(g_renderListBuf,(char *)0x0,0);
    MemUnlock();
    g_renderListBuf = (void *)0x0;
  }
  GfxRenderPacketsReset();
  GfxScreenCameraDestroy();
  return;
}

