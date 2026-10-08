// bdc 0x089445d0 UiStaffCreditCtor
#include "bdc.h"

/* Constructor of the staff credits screen `UiStaffCredit`, task id 3004 (0xbbc) (base
   `UiScreenCtor`, vtable `g_uiStaffCreditVtbl`, object size 0x940): clears the roll state,
   the track states, the 13 picture slots, the 502 per-line floats and the 20 line printers,
   resets `phaseStep`, sets frame mode 1 (`UiScreenSetFrameMode`), allocates the 4-byte
   `g_uiSharedAnims` block from low memory and zeroes it, and keeps the shared background
   (`UiScreenKeepSharedBg`). Returns `screen`. The packages and text are loaded later by the
   phase handlers (`UiStaffCreditLoadPhase`, `UiStaffCreditSetupPhase`).

   ## Notes
   - `unkfc` is not cleared. The `memset` runs on the allocation even if it returned NULL. */

UiScreen *UiStaffCreditCtor(UiScreen *screen)

{
  UiStaffCredit *credit = (UiStaffCredit *)screen;
  bool fromLow;
  void *anims;
  s32 i;

  UiScreenCtor(&screen->base);
  screen->base.vtable = g_uiStaffCreditVtbl;
  credit->langPackage = (IoLzsPackage *)0x0;
  credit->commonPackage = (IoLzsPackage *)0x0;
  credit->rollFrame = 0;
  credit->pauseTimer = 0;
  credit->lineTexts = (u32 *)0x0;
  credit->pictureIndex = 0;
  credit->unk84 = 0;
  credit->lineIndex = 0;
  credit->pictureDelay = 0;
  credit->lineCursor = 0;
  credit->pictureTrack = 0;
  credit->lineTrack = 0;
  credit->bgmTrack = 0;
  credit->unkf0 = 0;
  credit->unkf4 = 0;
  credit->lineCount = 0;
  for (i = 0; i < 13; i++) {
    credit->pictures[i].unk0 = 0;
    credit->pictures[i].state = 0;
  }
  for (i = 0; i < 0x1f6; i++) {
    credit->lineValues[i] = 0.0f;
  }
  for (i = 0; i < 20; i++) {
    credit->overlays[i] = (GfxSpriteLayer *)0x0;
  }
  screen->phaseStep = 0;
  UiScreenSetFrameMode(&screen->base, 1);
  MemLock();
  fromLow = MemIsAllocFromLow();
  MemSetAllocFromLow(true);
  anims = MemAlloc(sizeof(GfxFab *), (char *)0x0, 0);
  MemSetAllocFromLow(fromLow);
  MemUnlock();
  g_uiSharedAnims = (GfxFab **)anims;
  memset(anims, 0, sizeof(GfxFab *));
  UiScreenKeepSharedBg(screen);
  return screen;
}
