// bdc 0x0893d5c8 UiPasscodeCtor
#include "bdc.h"

/* Constructor of the passcode (symbol sequence) puzzle screen, task id 374 (0x176) (base
   `UiScreenCtor`, vtable `g_uiPasscodeVtbl`). Built by `CoreTaskNewByIdArg` (object size
   0x800, one-byte argument stored at `+0x7de`). Allocates a 0xac-byte sprite table, primes the
   fader, forces the pad's d-pad emulation and pauses the field task 500 (`CoreTaskFind` +
   `CoreTaskSetFlags``(t, 1)`). The player enters a sequence of symbols that is checked against
   the answer; the result goes to script global word `+0xc` (0 = correct, 1 = wrong), which the
   field script code (`GameEvent470CheckWait`) reads after the task is gone. */

UiScreen *UiPasscodeCtor(UiScreen *screen, u32 arg)

{
  UiPasscode *self = (UiPasscode *)screen;
  bool lowAlloc;
  void *sprites;
  PadState *pad;

  UiScreenCtor(&screen->base);
  (screen->base).vtable = &g_uiPasscodeVtbl;
  self->arg = (u8)arg;
  MemLock();
  lowAlloc = MemIsAllocFromLow();
  MemSetAllocFromLow(true);
  sprites = MemAlloc(43 * sizeof(GfxSprite *), (char *)0x0, 0);
  MemSetAllocFromLow(lowAlloc);
  MemUnlock();
  screen->data = sprites;
  UiScreenSetFrameMode(&screen->base, 1);
  self->unk6c = 0;
  if (!GfxFaderIsReady()) {
    GfxFaderSlotsInit((GfxFader *)0x0);
    GfxGetActiveFader()->sortKey = 20000.0f;
  }
  pad = screen->pad;
  self->unk70 = 0;
  self->savedStickEmu = pad->stickEmulatesDpad;
  pad->stickEmulatesDpad = 1;
  self->judgement = 0;
  {
    CoreTask *task = CoreTaskFind(500);
    if (task != (CoreTask *)0x0) {
      CoreTaskSetFlags(task, 1);
    }
  }
  return screen;
}
