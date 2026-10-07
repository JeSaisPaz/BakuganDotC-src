// bdc 0x0893eb50 UiPasscodeStartPressFlash
#include "bdc.h"

/* Starts the press flash (`UiFlashStart`, 2 frames, channel 0) on the focused button of
   `UiPasscode`: symbol sprite 1+index on the pad (onCommandRow == 0), or command
   sprite 0x0b+index otherwise. The index is read as `(&focusSymbol)[onCommandRow]`, so
   onCommandRow == 1 selects `command`. */

void UiPasscodeStartPressFlash(UiScreen *screen)

{
  UiPasscode *self = (UiPasscode *)screen;
  GfxSprite **sprites = (GfxSprite **)screen->data;
  s8 index = (&self->focusSymbol)[self->onCommandRow];

  if (self->onCommandRow == 0) {
    UiFlashStart(2.0f, sprites[index + 1], 1, 0);
    return;
  }
  UiFlashStart(2.0f, sprites[index + 0xb], 1, 0);
  return;
}
