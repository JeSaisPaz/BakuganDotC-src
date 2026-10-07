// bdc 0x08952238 UiEmptyScreen370Ctor
#include "bdc.h"

/* Constructor of task id 370 (0x172), an empty screen class: runs `UiScreenCtor` and installs
   vtable `0x08af4e24`; the object is a bare 0x6c-byte `UiScreen` with no data, phases or drawing
   of its own. */

UiScreen *UiEmptyScreen370Ctor(UiScreen *screen)

{
  UiScreenCtor(&screen->base);
  (screen->base).vtable = g_uiEmptyScreen370Vtbl;
  return screen;
}

