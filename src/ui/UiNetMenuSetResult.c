// bdc 0x0894e110 UiNetMenuSetResult
#include "bdc.h"

/* Sets the menu result of `UiNetMenu`: 0 when cancelled (`+0x2d0`); otherwise by
   the choice `+0x74`: 0 → 1 and profile word 7 cleared (`SaveProfileSetWord`), 1 → 2. */

void UiNetMenuSetResult(UiScreen *screen)

{
  UiNetMenu *menu = (UiNetMenu *)screen;
  SaveProfile *self;

  if (menu->cancelled == 0) {
    s8 choice = menu->choice;
    if (choice < 1) {
      if (choice >= 0) {
        UiSetMenuResult(screen, 1);
        self = SaveGetProfile();
        SaveProfileSetWord(self, 7, 0);
        return;
      }
    } else if (choice < 2) {
      UiSetMenuResult(screen, 2);
      return;
    }
  } else {
    UiSetMenuResult(screen, 0);
  }
  return;
}
