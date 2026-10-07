// bdc 0x088cb91c UiTalkBalloonCreatePrinter
#include "bdc.h"

/* Deletes the old text printer `+0x10` of the talk balloon (`UiTalkBalloonCtor`) and creates a
   new one (`UiTextPrinterCtor`, 0xf0 bytes) with the font `font`. */

void UiTalkBalloonCreatePrinter(UiTalkBalloon *self, char **font)

{
  bool fromLow;
  UiTextPrinter *mem;
  UiTextPrinter *printer;
  const VtblEntry *dtor;

  printer = self->printer;
  if (printer != NULL) {
    dtor = &printer->layer.vtbl[1];
    ((void (*)(void *, int))dtor->fn)((u8 *)printer + dtor->delta, 3);
    self->printer = NULL;
  }
  MemLock();
  fromLow = MemIsAllocFromLow();
  MemSetAllocFromLow(true);
  mem = MemAlloc(sizeof(UiTextPrinter), NULL, 0);
  MemSetAllocFromLow(fromLow);
  MemUnlock();
  printer = NULL;
  if (mem != NULL) {
    UiTextPrinterCtor(mem, 0, font);
    printer = mem;
  }
  self->printer = printer;
}
