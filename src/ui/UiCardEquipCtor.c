// bdc 0x089693dc UiCardEquipCtor
#include "bdc.h"

/* Constructor of UiCardEquip, the ability-card loadout sub-screen opened by `UiEquip`
   (loads `DWCardName_eu.bin`/`DWCardHelp_eu.bin`; slot count from profile word 0x16, slots in
   profile words 3..6); task id 303 (0x12f), object size 0x2b98. Runs `UiScreenCtor`, installs
   `g_uiCardEquipVtbl`, runs the class init (`UiCardEquipInitState`, `UiCardEquipInitTabMask`),
   allocates the per-screen data block (`data`, +0x1c) from the low heap: 0x1a4 bytes when
   `bakuganCount < 3`, else 0x31c; sets frame mode 1 (`UiScreenSetFrameMode`), primes the fader
   (sort key 20000 when it had to be created), clears the clear colour to opaque black, sets the
   pad's `stickEmulatesDpad`, clears both text printers and sets the shared-background hand-over
   flag `g_uiKeepSharedBg`. Returns the screen. */

UiScreen *UiCardEquipCtor(UiCardEquip *self)
{
  bool fromLow;
  void *data;
  UiTextPrinter **printer;
  s32 i;

  UiScreenCtor((CoreTask *)self);
  self->base.base.vtable = g_uiCardEquipVtbl;
  UiCardEquipInitState(self);
  UiCardEquipInitTabMask(self);
  if (self->bakuganCount < 3) {
    MemLock();
    fromLow = MemIsAllocFromLow();
    MemSetAllocFromLow(true);
    data = MemAlloc(0x1a4, NULL, 0);
    MemSetAllocFromLow(fromLow);
    MemUnlock();
    self->base.data = data;
  } else {
    MemLock();
    fromLow = MemIsAllocFromLow();
    MemSetAllocFromLow(true);
    data = MemAlloc(0x31c, NULL, 0);
    MemSetAllocFromLow(fromLow);
    MemUnlock();
    self->base.data = data;
  }
  UiScreenSetFrameMode((CoreTask *)self, 1);
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
  printer = &self->namePrinter; /* namePrinter, helpPrinter */
  for (i = 0; i < 2; i++) {
    printer[i] = NULL;
  }
  g_uiKeepSharedBg = 1;
  return &self->base;
}
