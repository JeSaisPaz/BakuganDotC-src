// bdc 0x08940c50 UiEmptyScreen380Ctor
#include "bdc.h"

/* Constructor of the empty screen class for task id 380 (0x17c) built by `CoreTaskNewById`
   (object size 0x6c, vtable `0x08af4bcc`): only runs `UiScreenCtor` and installs its vtable; the
   class adds no fields and overrides only the destructor (`UiEmptyScreen380Dtor`), keeping the
   base `UiScreenUpdate`/`UiScreenDraw` and `CoreTaskSetField`/`CoreTaskGetField`. */

UiScreen *UiEmptyScreen380Ctor(UiScreen *screen)

{
  UiScreenCtor(&screen->base);
  (screen->base).vtable = g_uiEmptyScreen380Vtbl;
  return screen;
}

