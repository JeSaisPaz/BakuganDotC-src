// bdc 0x089129b8 UiUpgradeDtor
#include "bdc.h"

/* Destructor (vtable slot 1) of the Bakugan upgrade screen (task id 490): reinstalls vtable
   `g_uiUpgradeVtbl`, waits for the GE, deletes the Bakugan model, the camera and the text
   printer through their virtual deleting destructors (flags 3, clearing each pointer), deletes the
   background animation list (`CoreObjectListDeleteAll(&bgAnimList)`), frees `bgData`, frees all GMO
   motions, then runs `UiScreenDtor``(screen, 0)` and frees the object when `flags & 1`. */

void UiUpgradeDtor(UiUpgrade *self, u32 flags)

{
  const VtblEntry *entry;
  void *bgData;

  if (self != (UiUpgrade *)0x0) {
    self->base.base.vtable = g_uiUpgradeVtbl;
    GfxWaitGeIdle();
    if (self->model != (void *)0x0) {
      entry = (const VtblEntry *)((CoreObject *)self->model)->vtable + 1;
      ((void (*)(void *, int))entry->fn)((char *)self->model + entry->delta, 3);
      self->model = (void *)0x0;
    }
    if (self->camera != (void *)0x0) {
      entry = (const VtblEntry *)((CoreNode *)self->camera)->vtable + 1;
      ((void (*)(void *, int))entry->fn)((char *)self->camera + entry->delta, 3);
      self->camera = (void *)0x0;
    }
    if (self->textPrinter != (UiTextPrinter *)0x0) {
      entry = (const VtblEntry *)self->textPrinter->layer.vtbl + 1;
      ((void (*)(void *, int))entry->fn)((char *)self->textPrinter + entry->delta, 3);
      self->textPrinter = (UiTextPrinter *)0x0;
    }
    CoreObjectListDeleteAll((CoreObjectList *)&self->base.bgAnimList);
    bgData = self->base.bgData;
    if (bgData != (void *)0x0) {
      MemLock();
      MemFree(bgData, (char *)0x0, 0);
      MemUnlock();
      self->base.bgData = (void *)0x0;
    }
    GmoMotionFreeAll(GmoMotionMgrGet(), 0);
    UiScreenDtor(&self->base, 0);
    if ((flags & 1) != 0) {
      MemLock();
      MemFree(self, (char *)0x0, 0);
      MemUnlock();
    }
  }
  return;
}
