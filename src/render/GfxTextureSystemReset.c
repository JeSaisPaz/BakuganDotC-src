// bdc 0x089f7634 GfxTextureSystemReset
#include "bdc.h"

/* Resets the texture system for a new display (`GfxRenderInit`): clears `g_textureList`,
   initialises `g_feedbackTexture` as a VRAM target named `"FeedBackTex"`
   (`GfxTextureInitVramTarget`) and registers the model library's allocators: main memory
   `GfxTexAllocMain`/`GfxTexFreeMain` and VRAM `GfxTexAllocVram`/`GfxTexFreeVram`
   (`GmoImageSetAllocator`/`GmoImageSetVramAllocator`, plus the aligned pools via `GmoImageHeapSetMainPool` (align 0x10) and
   `GmoImageHeapSetVramPool` (align 0x40)). */

void GfxTextureSystemReset(void)

{
  GfxTextureListClear();
  g_textureList = (void *)0x0;
  GfxTextureInitVramTarget(&g_feedbackTexture,"FeedBackTex",(void *)0x0);
  GmoImageSetAllocator(GfxTexAllocMain,GfxTexFreeMain);
  GmoImageSetVramAllocator(GfxTexAllocVram,GfxTexFreeVram);
  GmoImageHeapSetMainPool(GfxTexAllocMain,GfxTexFreeMain,0x10,'\0');
  GmoImageHeapSetVramPool(GfxTexAllocVram,GfxTexFreeVram,0x40,'\0');
  return;
}

