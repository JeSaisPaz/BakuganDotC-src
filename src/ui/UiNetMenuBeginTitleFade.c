// bdc 0x0894e7c4 UiNetMenuBeginTitleFade
#include "bdc.h"

/* Starts the fade of the title sprite 0 of `UiNetMenu` (t `+0x94` = 0; start alpha
   `+0x98` and help alpha `+0x2e0` = 0 when opening, 1 when closing). */

void UiNetMenuBeginTitleFade(UiScreen *screen, u8 closing)

{
  UiNetMenu *menu = (UiNetMenu *)screen;

  if (closing == 0) {
    menu->titleHelpAlpha = 0.0f;
    menu->titleFadeT = 0.0f;
    menu->titleFadeAlpha = 0.0f;
    return;
  }
  menu->titleFadeT = 0.0f;
  menu->titleHelpAlpha = 1.0f;
  menu->titleFadeAlpha = 1.0f;
}
