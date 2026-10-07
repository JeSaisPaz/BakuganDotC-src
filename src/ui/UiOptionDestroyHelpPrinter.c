// bdc 0x0896fd64 UiOptionDestroyHelpPrinter
#include "bdc.h"

/* Destroys the help text printer of `UiOption` (`+0xbbc`) through its virtual
   destructor (delete flag 3) and clears the pointer; called by `UiOptionDtor`. */

void UiOptionDestroyHelpPrinter(UiOption *self)

{
  UiTextPrinter *printer;

  printer = self->helpPrinter;
  if (printer != (UiTextPrinter *)0x0) {
    const VtblEntry *dtor = &printer->layer.vtbl[1];
    ((void (*)(void *, int))dtor->fn)((u8 *)printer + dtor->delta,3);
    self->helpPrinter = (UiTextPrinter *)0x0;
  }
  return;
}
