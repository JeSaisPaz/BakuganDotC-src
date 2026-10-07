// bdc 0x08982628 UiCollectionCardDtor
#include "bdc.h"

/* Destructor (vtable slot 1) of the UiCollectionCard screen (task id 313): reinstalls vtable
   `0x08af4fac`, waits for the GE, deletes its owned sprite/model objects through their virtual
   destructors; then `UiScreenDtor``(this, 0)` and frees the object when `flags & 1`. */

void UiCollectionCardDtor(UiCollectionCard *self, u32 flags)

{
  UiTextPrinter *printer;
  const VtblEntry *vt;
  
  if (self != (UiCollectionCard *)0x0) {
    (self->base).base.vtable = g_uiCollectionCardVtbl;
    GfxWaitGeIdle();
    printer = self->helpPrinter;
    if (printer != (UiTextPrinter *)0x0) {
      vt = (printer->layer).vtbl;
      ((void (*)(void *, int))vt[1].fn)((char *)printer + vt[1].delta,3);
      self->helpPrinter = (UiTextPrinter *)0x0;
    }
    UiScreenDtor(&self->base,0);
    if ((flags & 1) != 0) {
      MemLock();
      MemFree(self,(char *)0x0,0);
      MemUnlock();
    }
  }
  return;
}

