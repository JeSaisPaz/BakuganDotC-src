// bdc 0x0895767c UiEquipDtor
#include "bdc.h"

/* Destructor (vtable slot 1) of the UiEquip screen (task id 302): reinstalls vtable
   `g_uiEquipVtbl`, waits for the GE, resets the frame mode to 1, frees the slot camera array
   `slotCameras` (its `CxxVecBlock` new[] block, cookie header in front), runs `UiEquipFreeModels`, clears
   `g_gfxActiveCamera`, deletes `namePrinter` and `helpPrinter` through their virtual destructors
   (flag 3), stores its task id in `g_lastScreenTaskId` and profile word 0x1d; then
   `UiScreenDtor``(this, 0)` and frees the object when `flags & 1`. */

void UiEquipDtor(UiEquip *self, u32 flags)

{
  UiTextPrinter **printers;
  const VtblEntry *entry;
  s32 i;

  if (self != (UiEquip *)0x0) {
    self->base.base.vtable = g_uiEquipVtbl;
    GfxWaitGeIdle();
    UiScreenSetFrameMode((CoreTask *)self, 1);
    if (self->slotCameras != (GfxCamera *)0x0) {
      MemLock();
      MemFree((CxxVecBlock *)self->slotCameras - 1, (char *)0x0, 0); /* new[] cookie header */
      MemUnlock();
      self->slotCameras = (GfxCamera *)0x0;
    }
    UiEquipFreeModels(self);
    g_gfxActiveCamera = (GfxCamera *)0x0;
    printers = &self->namePrinter;
    for (i = 0; i < 2; i++) {
      if (printers[i] != (UiTextPrinter *)0x0) {
        entry = &printers[i]->layer.vtbl[1];
        ((void (*)(void *, s32))entry->fn)((u8 *)printers[i] + entry->delta, 3);
        printers[i] = (UiTextPrinter *)0x0;
      }
    }
    g_lastScreenTaskId = self->base.base.id;
    SaveProfileSetWord(SaveGetProfile(), 0x1d, self->base.base.id);
    UiScreenDtor(&self->base, 0);
    if ((flags & 1) != 0) {
      MemLock();
      MemFree(self, (char *)0x0, 0);
      MemUnlock();
    }
  }
  return;
}
