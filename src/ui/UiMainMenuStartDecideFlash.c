// bdc 0x089a8d84 UiMainMenuStartDecideFlash
#include "bdc.h"

/* Starts the 2-frame decide flash (`UiFlashStart`, slot 0) on the selected item's sprite
   `data+0x14+item*4`. */

void UiMainMenuStartDecideFlash(UiMainMenu *self)

{
  UiFlashStart(2.0f, ((GfxSprite **)(self->base).data)[5 + self->cursor], 0, 0);
  return;
}

