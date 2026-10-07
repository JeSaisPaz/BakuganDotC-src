// bdc 0x08a1f840 GfxGuCallbackSig
#include "bdc.h"

/* GE signal callback of the statically linked libgu (pspsdk `callbackSig`, registered by
   `sceGuInit` through `sceGeSetCallback`): records `id & 0xffff` in the 16-entry signal history
   (`signalHistory`, index `signalOffset` incremented), calls the user signal handler if set as
   `(id & 0xffff, listPc)`, and sets bit 1 of the `SceGuSignal` event flag. */

void GfxGuCallbackSig(int id, void *settings, void *listPc)
{
  GuSettings *s = (GuSettings *)settings;
  int offset = s->signalOffset;
  void (*handler)(int, void *) = s->signalHandler;

  s->signalHistory[offset % 16] = (s16)(id & 0xffff);
  s->signalOffset = offset + 1;
  if (handler != 0) {
    handler(id & 0xffff, listPc);
  }
  sceKernelSetEventFlag(s->signalEventFlag, 1);
}
