// bdc 0x0890e1dc UiEmptyScreen460Ctor
#include "bdc.h"

/* Constructor of the empty screen class for task id 460 (0x1cc) built by `CoreTaskNewById`
   (object size 0x6c, vtable `0x08af484c`): only runs `UiScreenCtor` and installs its vtable; the
   class adds no fields and overrides only the destructor (`UiEmptyScreen460Dtor`), keeping the
   base `UiScreenUpdate`/`UiScreenDraw` and `CoreTaskSetField`/`CoreTaskGetField`. */

UiScreen *UiEmptyScreen460Ctor(UiScreen *screen)

{
  UiScreenCtor(&screen->base);
  (screen->base).vtable = g_uiEmptyScreen460Vtbl;
  return screen;
}

