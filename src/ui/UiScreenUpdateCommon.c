// bdc 0x08909d64 UiScreenUpdateCommon
#include "bdc.h"

/* Common tail of every derived screen update: when the close request `+0x4c` is set, removes and
   deletes the task (`CoreTaskRemove``(screen, true)`); otherwise, when the sprite layer `+0x18`
   exists and its byte/word `+0x1c` is set, runs the palette pulse (`UiScreenPulsePalettes`). */

void UiScreenUpdateCommon(UiScreen *screen)

{
  if (screen->closeRequested == '\0') {
    if ((screen->spriteLayer != (GfxSpriteLayer *)0x0) &&
       (screen->spriteLayer->head != (GfxSprite *)0x0)) {
      UiScreenPulsePalettes(screen);
    }
    return;
  }
  CoreTaskRemove(&screen->base,true);
  return;
}

