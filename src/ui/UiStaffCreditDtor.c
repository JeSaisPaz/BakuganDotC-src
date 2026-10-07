// bdc 0x08944718 UiStaffCreditDtor
#include "bdc.h"

/* Destructor (vtable slot 1) of the staff credits screen (`UiStaffCredit`, task id 3004):
   reinstalls `g_uiStaffCreditVtbl`, waits for the GE, deletes the common and language packages
   (`commonPackage` first) and the 20 credit-line printers (`overlays`) through their virtual
   destructors, stores its task id in `g_lastScreenTaskId`; then `UiScreenDtor``(screen, 0)`
   and frees the object when `flags & 1`. */

void UiStaffCreditDtor(UiScreen *screen, u32 flags)

{
  UiStaffCredit *credit = (UiStaffCredit *)screen;
  const VtblEntry *dtor;
  s32 i;

  if (screen != (UiScreen *)0x0) {
    screen->base.vtable = g_uiStaffCreditVtbl;
    GfxWaitGeIdle();
    if (credit->commonPackage != (IoLzsPackage *)0x0) {
      dtor = (const VtblEntry *)credit->commonPackage->base.vtable + 1;
      ((void (*)(void *, s32))dtor->fn)((u8 *)credit->commonPackage + dtor->delta, 3);
      credit->commonPackage = (IoLzsPackage *)0x0;
    }
    if (credit->langPackage != (IoLzsPackage *)0x0) {
      dtor = (const VtblEntry *)credit->langPackage->base.vtable + 1;
      ((void (*)(void *, s32))dtor->fn)((u8 *)credit->langPackage + dtor->delta, 3);
      credit->langPackage = (IoLzsPackage *)0x0;
    }
    for (i = 0; i < 20; i++) {
      GfxSpriteLayer *layer = credit->overlays[i];

      if (layer != (GfxSpriteLayer *)0x0) {
        dtor = &layer->vtbl[1];
        ((void (*)(void *, s32))dtor->fn)((u8 *)layer + dtor->delta, 3);
        credit->overlays[i] = (GfxSpriteLayer *)0x0;
      }
    }
    g_lastScreenTaskId = screen->base.id;
    UiScreenDtor(screen, 0);
    if ((flags & 1) != 0) {
      MemLock();
      MemFree(screen, (char *)0x0, 0);
      MemUnlock();
    }
  }
  return;
}
