// bdc 0x0894eed0 UiNetMenuStartConfirmFlash
#include "bdc.h"

/* Starts the 2-frame confirm flash (`UiFlashStart`, channel 0) on button sprite 4+choice of
   `UiNetMenu`. */

void UiNetMenuStartConfirmFlash(UiScreen *screen)

{
  UiNetMenu *menu = (UiNetMenu *)screen;

  UiFlashStart(2.0f, ((GfxSprite **)menu->base.data)[menu->choice + 4], 0, 0);
}
