// bdc 0x08971c68 UiOptionStartButtonPress
#include "bdc.h"

/* Starts the press animation (`UiFlashStart`, 4 frames) of the selected button of
   `UiOption`. */

void UiOptionStartButtonPress(UiOption *self)

{
  GfxSprite **sprites = (GfxSprite **)self->base.data;

  UiFlashStart(4.0f, sprites[0x2c + (signed char)self->cursor], 0, 0);
  return;
}
