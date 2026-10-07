// bdc 0x089a81c4 UiMainMenuShowGuestHelp
#include "bdc.h"

/* For the guest of a net session (profile flag 0, player index 1) shows help message 5
   (`UiHelpLineShow`) just above sprite `data[1]`. */

void UiMainMenuShowGuestHelp(UiMainMenu *self)

{
  GfxSprite *sprite;

  if (SaveGetProfileFlag0() != 0 && NetGetLocalPlayerIndex() == 1) {
    sprite = ((GfxSprite **)self->base.data)[1];
    UiHelpLineShow(sprite->posX, sprite->posY - 6.0f, 5);
  }
}
