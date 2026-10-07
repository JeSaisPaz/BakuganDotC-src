// bdc 0x0893ec08 UiPasscodeRemoveSymbol
#include "bdc.h"

/* Removes the last entered symbol of `UiPasscode` (decrements `+0x7fc`, sets that
   slot of `+0x7f2` to 0xff). */

void UiPasscodeRemoveSymbol(UiPasscode *screen)

{
  screen->entryCount = screen->entryCount - 1;
  screen->entry[screen->entryCount] = 0xff;
}
