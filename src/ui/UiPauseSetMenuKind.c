// bdc 0x08910898 UiPauseSetMenuKind
#include "bdc.h"

/* Stores the pause menu `kind` of the pause screen (`UiPauseCtor`). For kind 6 with
   `g_profileFlag0` set, a net-play manager present (`NetPlayHasManager`) and no `netPad` yet,
   sets the manager's `remotePad->stickEmulatesDpad` to 1, allocates a 0x50-byte pad object from
   the low heap (`MemAlloc`, `PadBaseCtor`) into `netPad`, clears its state
   (`PadSetExternalState` with NULL masks/stick) and makes it the screen's `pad`. */

void UiPauseSetMenuKind(UiPause *self, int kind)
{
  bool useNetPad;
  bool wasLow;
  PadState *mem;
  PadState *pad;
  NetPlay *net;

  self->kind = kind;
  useNetPad = false;
  switch (kind) {
  case 6:
    if (SaveGetProfileFlag0() != 0 && NetPlayHasManager() && self->base.netPad == NULL) {
      useNetPad = true;
    }
    break;
  default:
    break;
  }
  if (useNetPad) {
    net = (NetPlay *)NetPlayGetManager();
    net->remotePad->stickEmulatesDpad = 1;
    MemLock();
    wasLow = MemIsAllocFromLow();
    MemSetAllocFromLow(true);
    mem = (PadState *)MemAlloc(0x50 /* PSP: pad object, smaller than PadState (0x5c) */, NULL, 0);
    MemSetAllocFromLow(wasLow);
    MemUnlock();
    pad = NULL;
    if (mem != NULL) {
      PadBaseCtor(mem);
      pad = mem;
    }
    self->base.netPad = pad;
    PadSetExternalState(pad, NULL, NULL);
    self->base.pad = self->base.netPad;
  }
}
