// bdc 0x0890ff74 UiEmptyScreen440Ctor
#include "bdc.h"

/* Constructor of the empty screen class for task id 440 (0x1b8) built by `CoreTaskNewById`
   (object size 0x6c, vtable `0x08af48f4`): only runs `UiScreenCtor` and installs its vtable; the
   class adds no fields and overrides only the destructor (`UiEmptyScreen440Dtor`), keeping the
   base `UiScreenUpdate`/`UiScreenDraw` and `CoreTaskSetField`/`CoreTaskGetField`. */

UiScreen *UiEmptyScreen440Ctor(UiScreen *screen)

{
  UiScreenCtor(&screen->base);
  (screen->base).vtable = g_uiEmptyScreen440Vtbl;
  return screen;
}

