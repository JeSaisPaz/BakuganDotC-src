// bdc 0x089e0bd0 GmoSystemInit
#include "bdc.h"

/* Start-up of the GMO model/motion runtime, called from `GfxRenderInit`: configures the model
   library's block heap pools (`GmoHeapSetMainPool` with `GmoAlloc`/`GmoFree`, alignment 0x10;
   `GmoHeapSetVramPool` with `GmoAllocAligned`/`GmoFreeAligned`, alignment 0x40), installs the
   texture hooks (`GmoInstallTextureHooks`), creates the motion manager
   (`GmoMotionMgrGetOrCreate`), clears the globals `0x08ac5c65`, `0x08ac5c66`, `0x08b02150/54/58`
   and sets the motion time scale to 1.0 (`GfxSetMotionTimeScale`). */

void GmoSystemInit(void)

{
  GmoHeapSetMainPool(GmoAlloc,GmoFree,0x10,'\0');
  GmoHeapSetVramPool(GmoAllocAligned,GmoFreeAligned,0x40,'\0');
  GmoInstallTextureHooks();
  GmoMotionMgrGetOrCreate();
  g_gmoMotionPaused = '\0';
  g_coreObjectDeferDeleteList.tail = (CoreObject *)0x0;
  g_coreObjectDeferDeleteList.head = (CoreObject *)0x0;
  g_coreObjectDeferDeleteList.count = 0;
  g_gmoDrawDisabled = '\0';
  GfxSetMotionTimeScale(1.0);
  return;
}

