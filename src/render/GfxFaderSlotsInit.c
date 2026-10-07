// bdc 0x089ed9f4 GfxFaderSlotsInit
#include "bdc.h"

/* Lazily creates the fader system: allocates the two-word slot table `g_faderSlots` (low heap),
   constructs the default fader (0x70 bytes, `GfxScreenFaderCtor`) in slot 0 and sets slot 1 (active) to
   `activeFader`, or to slot 0 when NULL. Also makes sure the core task `0x274c` (the fader updater)
   exists, creating it with `CoreTaskCreate(0x274c, 100)`. Returns the active fader. Called by
   `ScriptOpFade` when no fader exists yet. */

GfxFader * GfxFaderSlotsInit(GfxFader *activeFader)

{
  bool wasLow;
  void **slots;
  GfxScreenFader *fader;

  if (g_faderSlots == NULL) {
    MemLock();
    wasLow = MemIsAllocFromLow();
    MemSetAllocFromLow(true);
    slots = MemAlloc(8, NULL, 0);
    MemSetAllocFromLow(wasLow);
    MemUnlock();
    g_faderSlots = slots;
    memset(slots, 0, 8);

    MemLock();
    wasLow = MemIsAllocFromLow();
    MemSetAllocFromLow(true);
    fader = MemAlloc(0x70, NULL, 0);
    MemSetAllocFromLow(wasLow);
    MemUnlock();
    if (fader != NULL) {
      GfxScreenFaderCtor(fader);
    }
    g_faderSlots[0] = fader;
    GfxFaderNop();
    if (activeFader == NULL) {
      g_faderSlots[1] = g_faderSlots[0];
    } else {
      g_faderSlots[1] = activeFader;
    }
  }
  if (CoreTaskFind(0x274c) == NULL) {
    CoreTaskCreate(0x274c, 100);
  }
  return g_faderSlots[1];
}
