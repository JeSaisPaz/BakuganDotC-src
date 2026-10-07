// bdc 0x0894ef44 UiNetMenuBeginHelpWindowFade
#include "bdc.h"

/* Starts the fade of the help window sprite 0x0e of `UiNetMenu` (t `+0x2c4` = 0,
   start alpha `+0x2c8` = 0 opening / 1 closing). */

void UiNetMenuBeginHelpWindowFade(UiScreen *screen, u8 closing)

{
  UiNetMenu *menu = (UiNetMenu *)screen;

  if (closing == 0) {
    menu->helpFadeT = 0.0f;
    menu->helpFadeAlpha = 0.0f;
    return;
  }
  menu->helpFadeT = 0.0f;
  menu->helpFadeAlpha = 1.0f;
}
