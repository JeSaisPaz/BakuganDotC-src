// bdc 0x08944470 UiEmptyScreen400Ctor
#include "bdc.h"

/* Constructor of the empty screen class for task id 400 (0x190) built by `CoreTaskNewById`
   (object size 0x6c, vtable `0x08af4cd4`): only runs `UiScreenCtor` and installs its vtable; the
   class adds no fields and overrides only the destructor (`UiEmptyScreen400Dtor`), keeping the
   base `UiScreenUpdate`/`UiScreenDraw` and `CoreTaskSetField`/`CoreTaskGetField`. */

UiScreen *UiEmptyScreen400Ctor(UiScreen *screen)

{
  UiScreenCtor(&screen->base);
  (screen->base).vtable = g_uiEmptyScreen400Vtbl;
  return screen;
}

