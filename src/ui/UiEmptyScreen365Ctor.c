// bdc 0x08996308 UiEmptyScreen365Ctor
#include "bdc.h"

/* Constructor of task id 365 (0x16d), an empty screen class: runs `UiScreenCtor` and installs
   vtable `0x08af508c`; the object is a bare 0x6c-byte `UiScreen` with no data, phases or drawing
   of its own. */

UiScreen *UiEmptyScreen365Ctor(UiScreen *screen)

{
  UiScreenCtor(&screen->base);
  (screen->base).vtable = g_uiEmptyScreen365Vtbl;
  return screen;
}

