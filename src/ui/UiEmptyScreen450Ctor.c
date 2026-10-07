// bdc 0x0891001c UiEmptyScreen450Ctor
#include "bdc.h"

/* Constructor of the empty screen class for task id 450 (0x1c2) built by `CoreTaskNewById`
   (object size 0x6c, vtable `0x08af492c`): only runs `UiScreenCtor` and installs its vtable; the
   class adds no fields and overrides only the destructor (`UiEmptyScreen450Dtor`), keeping the
   base `UiScreenUpdate`/`UiScreenDraw` and `CoreTaskSetField`/`CoreTaskGetField`. */

UiScreen *UiEmptyScreen450Ctor(UiScreen *screen)

{
  UiScreenCtor(&screen->base);
  (screen->base).vtable = g_uiEmptyScreen450Vtbl;
  return screen;
}

