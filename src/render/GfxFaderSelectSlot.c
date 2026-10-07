// bdc 0x089ed980 GfxFaderSelectSlot
#include "bdc.h"

/* Makes fader `slot` (pointer table at `0x08ac5d98`) the active fader. The current active fader's
   state is copied into a 0x60-byte stack snapshot (`GfxFaderBaseCtor`/`GfxFaderCopyState`),
   `GfxSetActiveFader` is switched to the new slot, the snapshot is copied into it and
   `GfxFaderBaseDtor(snapshot, 2)` finishes the hand-over, so colours and timing carry over between
   fader slots. Called by `ScriptOpFade` cmd 7. */

void GfxFaderSelectSlot(int slot)
{
    GfxFader snapshot __attribute__((aligned(16)));

    GfxFaderBaseCtor(&snapshot);
    GfxFaderCopyState(&snapshot, GfxGetActiveFader());
    GfxSetActiveFader(g_faderTable[slot]);
    GfxFaderCopyState(GfxGetActiveFader(), &snapshot);
    GfxFaderBaseDtor(&snapshot, 2);
}
