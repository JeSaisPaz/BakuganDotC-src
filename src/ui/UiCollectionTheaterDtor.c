// bdc 0x08987c44 UiCollectionTheaterDtor
#include "bdc.h"

/* Destructor (vtable slot 1) of the UiCollectionTheater screen (task id 315): reinstalls vtable
   `0x08af4fe4`, waits for the GE; then `UiScreenDtor``(this, 0)` and frees the object when `flags
   & 1`. */

void UiCollectionTheaterDtor(UiScreen *screen, u32 flags)

{
  if (screen != (UiScreen *)0x0) {
    (screen->base).vtable = g_uiCollectionTheaterVtbl;
    GfxWaitGeIdle();
    UiScreenDtor(screen,0);
    if ((flags & 1) != 0) {
      MemLock();
      MemFree(screen,(char *)0x0,0);
      MemUnlock();
    }
  }
  return;
}

