// bdc 0x0896fc48 UiOptionCtor
#include "bdc.h"

/* Constructor of UiOption, the options screen (`option_battle_t_07`, `option_sol00`); task id 304
   (0x130), object size 0xc24. Runs `UiScreenCtor`, installs vtable `g_uiOptionVtbl`, allocates
   the 0xec-byte per-screen data block (`data`, +0x1c) from the low heap, sets frame mode 0
   (`UiScreenSetFrameMode`), primes the fader, clears the clear colour to black and sets the pad's
   `stickEmulatesDpad`; class init in `UiOptionInitState`, `UiOptionCreateHelpPrinter`; sets the
   shared-background hand-over flag `g_uiKeepSharedBg` to 1 and clears `netFlag`. Returns `self`. */

UiOption *UiOptionCtor(UiOption *self)
{
  bool fromLow;
  void *data;

  UiScreenCtor((CoreTask *)self);
  self->base.base.vtable = g_uiOptionVtbl;
  UiOptionInitState(self);

  MemLock();
  fromLow = MemIsAllocFromLow();
  MemSetAllocFromLow(true);
  data = MemAlloc(0x3b * sizeof(GfxSprite *), NULL, 0);
  MemSetAllocFromLow(fromLow);
  MemUnlock();
  self->base.data = data;

  UiScreenSetFrameMode((CoreTask *)self, 0);
  self->unk6c = 0;
  if (!GfxFaderIsReady()) {
    GfxFaderSlotsInit(NULL);
    GfxGetActiveFader()->sortKey = 20000.0f;
  }
  self->unk70 = 0;

  g_gfxDisplay->clearColor[0] = 0.0f;
  g_gfxDisplay->clearColor[1] = 0.0f;
  g_gfxDisplay->clearColor[2] = 0.0f;
  g_gfxDisplay->clearColor[3] = 1.0f;
  self->base.pad->stickEmulatesDpad = 1;
  g_uiKeepSharedBg = 1;
  UiOptionCreateHelpPrinter(self);
  self->netFlag = 0;
  return self;
}
