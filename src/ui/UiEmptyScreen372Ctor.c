// bdc 0x08937c40 UiEmptyScreen372Ctor
#include "bdc.h"

/* Constructor of task id 372 (0x174), an empty screen class: runs `UiScreenCtor` and installs
   vtable `0x08af4aec`; the object is a bare 0x6c-byte `UiScreen` with no data, phases or drawing
   of its own. */

UiScreen *UiEmptyScreen372Ctor(UiScreen *screen)

{
  UiScreenCtor(&screen->base);
  (screen->base).vtable = g_uiEmptyScreen372Vtbl;
  return screen;
}

