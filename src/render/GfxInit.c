// bdc 0x089ce980 GfxInit
#include "bdc.h"

/* Initialises the render system: destroys an existing `g_gfxDisplay` (`GfxDisplayDtor`, 3),
   allocates a new 0xa0-byte `GfxDisplay` from the low heap, constructs it (`GfxDisplayCtor`),
   stores it in `g_gfxDisplay`, brings up the GE (`GfxDisplaySetup`) and finishes with
   `GfxRenderInit`. */

void GfxInit(void)

{
  bool fromLow;
  GfxDisplay *display;
  GfxDisplay *display_00;
  
  if (g_gfxDisplay != (GfxDisplay *)0x0) {
    GfxDisplayDtor(g_gfxDisplay,3);
    g_gfxDisplay = (GfxDisplay *)0x0;
  }
  MemLock();
  fromLow = MemIsAllocFromLow();
  MemSetAllocFromLow(true);
  display = MemAlloc(0xa0,(char *)0x0,0);
  MemSetAllocFromLow(fromLow);
  MemUnlock();
  display_00 = (GfxDisplay *)0x0;
  if (display != (GfxDisplay *)0x0) {
    GfxDisplayCtor(display);
    display_00 = display;
  }
  g_gfxDisplay = display_00;
  GfxDisplaySetup(display_00);
  GfxRenderInit();
  return;
}

