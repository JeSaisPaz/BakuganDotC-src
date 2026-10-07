// bdc 0x0890e094 UiSharedBgCtor
#include "bdc.h"

/* Constructor of task id 320 (0x140), the shared-background keeper (object size 0x6c = a bare
   `UiScreen`): runs `UiScreenCtor` and installs vtable `0x08af4814`. While a screen hands over
   to the next one (`UiScreenKeepSharedBg`) this task keeps animating and drawing the shared
   background list `g_uiSharedAnimList`. */

UiScreen *UiSharedBgCtor(UiScreen *screen)

{
  UiScreenCtor(&screen->base);
  (screen->base).vtable = &g_uiSharedBgVtable;
  return screen;
}

