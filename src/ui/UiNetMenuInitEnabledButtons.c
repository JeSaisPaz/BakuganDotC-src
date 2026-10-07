// bdc 0x0894dfe4 UiNetMenuInitEnabledButtons
#include "bdc.h"

/* Marks both menu buttons of `UiNetMenu` (host, join) as enabled (`+0x2d1`,
   `+0x2d2` = 1). */

void UiNetMenuInitEnabledButtons(UiScreen *screen)

{
  UiNetMenu *menu = (UiNetMenu *)screen;
  s8 init[2];
  int i;

  init[0] = 1;
  init[1] = 1;
  i = 0;
  do {
    menu->enabled[i] = init[i];
    i++;
  } while (i < 2);
}
