// bdc 0x08a1f8c0 GfxGuCallbackFin
#include "bdc.h"

/* GE finish callback of the statically linked libgu (pspsdk `callbackFin`, registered by
   `sceGuInit` through `sceGeSetCallback`): calls the user finish handler `settings+4` as `(id &
   0xffff, listPc)` when one is set. */

void GfxGuCallbackFin(int id, void *settings, void *listPc)

{
  void (*handler)(int, void *) = ((GuSettings *)settings)->finishHandler;
  if (handler != 0) {
    handler(id & 0xffff, listPc);
  }
  return;
}
