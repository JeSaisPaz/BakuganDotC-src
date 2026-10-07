// bdc 0x089b1af0 UiEmptyScreen360Ctor
#include "bdc.h"

/* Constructor of task id 360 (0x168), an empty screen class: runs `UiScreenCtor` and installs
   vtable `0x08af51a4`; the object is a bare 0x6c-byte `UiScreen` with no data, phases or drawing
   of its own. */

UiScreen *UiEmptyScreen360Ctor(UiScreen *screen)

{
  UiScreenCtor(&screen->base);
  (screen->base).vtable = g_uiEmptyScreen360Vtbl;
  return screen;
}

