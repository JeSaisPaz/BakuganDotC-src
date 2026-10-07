// bdc 0x0893ebec UiPasscodeAddSymbol
#include "bdc.h"

/* Appends the focused symbol `+0x75` to the entry `+0x7f2` of `UiPasscode` and
   increments the count `+0x7fc`. */

void UiPasscodeAddSymbol(UiPasscode *screen)

{
  u8 count;

  count = screen->entryCount;
  screen->entry[count] = screen->focusSymbol;
  screen->entryCount = count + 1;
}
