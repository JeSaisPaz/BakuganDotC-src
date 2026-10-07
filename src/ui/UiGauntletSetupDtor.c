// bdc 0x08931c30 UiGauntletSetupDtor
#include "bdc.h"

/* Destructor (vtable slot 1) of the gauntlet card setup screen (task id 373): reinstalls
   `g_uiGauntletSetupVtbl`, waits for the GE, frees its models (`UiGauntletSetupFreeModels`)
   and every motion (`GmoMotionFreeAll`), frees the `views` block, clears `g_gfxActiveCamera`,
   deletes the name and help text printers (virtual dtor, flags 3) and stores its id in
   `g_lastScreenTaskId`. Then `UiScreenDtor``(screen, 0)`; frees the object when `flags & 1`. */

void UiGauntletSetupDtor(UiGauntletSetup *self, u32 flags)
{
  UiTextPrinter **slots[2];
  s32 i;

  if (self != (UiGauntletSetup *)0x0) {
    (self->base).base.vtable = g_uiGauntletSetupVtbl;
    GfxWaitGeIdle();
    UiGauntletSetupFreeModels(self);
    GmoMotionFreeAll(GmoMotionMgrGet(), 0);
    if (self->views != (void *)0x0) {
      MemLock();
      MemFree((CxxVecBlock *)self->views - 1, (char *)0x0, 0); /* new[] cookie header */
      MemUnlock();
      self->views = (void *)0x0;
    }
    g_gfxActiveCamera = (GfxCamera *)0x0;
    /* the asm walks both slots with a 0x224 stride: namePrinter (+0xcb0), helpPrinter (+0xed4) */
    slots[0] = &self->namePrinter;
    slots[1] = &self->helpPrinter;
    for (i = 0; i < 2; i++) {
      UiTextPrinter *printer = *slots[i];

      if (printer != (UiTextPrinter *)0x0) {
        const VtblEntry *dtor = &printer->layer.vtbl[1];

        ((void (*)(void *, s32))dtor->fn)((u8 *)printer + dtor->delta, 3);
        *slots[i] = (UiTextPrinter *)0x0;
      }
    }
    g_lastScreenTaskId = (self->base).base.id;
    UiScreenDtor(&self->base, 0);
    if ((flags & 1) != 0) {
      MemLock();
      MemFree(self, (char *)0x0, 0);
      MemUnlock();
    }
  }
}
