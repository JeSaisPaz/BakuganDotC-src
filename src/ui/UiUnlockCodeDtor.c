// bdc 0x089924b0 UiUnlockCodeDtor
#include "bdc.h"

/* Destructor (vtable slot 1) of the UiUnlockCode screen (task id 316): reinstalls
   `g_unlockCodeVtbl`, waits for the GE, deletes the digit and key text printers and the common
   and screen packages through their virtual destructors (slot 1, flags 3), stores the task id in
   profile word 0x1d (`SaveProfileSetWord`), then `UiScreenDtor``(this, 0)` and frees the object
   when `flags & 1`. Does nothing for NULL. */

void UiUnlockCodeDtor(UiUnlockCode *self, u32 flags)

{
  const VtblEntry *e;
  SaveProfile *profile;

  if (self != (UiUnlockCode *)0x0) {
    self->base.base.vtable = g_unlockCodeVtbl;
    GfxWaitGeIdle();
    if (self->digitText != (UiTextPrinter *)0x0) {
      e = self->digitText->layer.vtbl + 1;
      ((void (*)(void *, int))e->fn)((u8 *)self->digitText + e->delta, 3);
      self->digitText = (UiTextPrinter *)0x0;
    }
    if (self->keyText != (UiTextPrinter *)0x0) {
      e = self->keyText->layer.vtbl + 1;
      ((void (*)(void *, int))e->fn)((u8 *)self->keyText + e->delta, 3);
      self->keyText = (UiTextPrinter *)0x0;
    }
    if (self->commonPackage != (void *)0x0) {
      e = (const VtblEntry *)((IoLzsPackage *)self->commonPackage)->base.vtable + 1;
      ((void (*)(void *, int))e->fn)((u8 *)self->commonPackage + e->delta, 3);
      self->commonPackage = (void *)0x0;
    }
    if (self->package != (void *)0x0) {
      e = (const VtblEntry *)((IoLzsPackage *)self->package)->base.vtable + 1;
      ((void (*)(void *, int))e->fn)((u8 *)self->package + e->delta, 3);
      self->package = (void *)0x0;
    }
    profile = SaveGetProfile();
    SaveProfileSetWord(profile, 0x1d, (u32)self->base.base.id);
    UiScreenDtor(&self->base, 0);
    if ((flags & 1) != 0) {
      MemLock();
      MemFree(self, (char *)0x0, 0);
      MemUnlock();
    }
  }
  return;
}
