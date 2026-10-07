// bdc 0x0893ec6c UiPasscodeShowButtonPrompts
#include "bdc.h"

/* Shows (`visible`) or hides the button-prompt sprites 0x27–0x2a of
   `UiPasscode`: showing sets visibility flag bit 0 and alpha 1, after giving
   sprite 0x27 button icon 2 and 0x28 icon 1 (`UiSetButtonIcon`); hiding clears bit 0. */

void UiPasscodeShowButtonPrompts(UiScreen *screen, u8 visible)
{
  int i;

  if (visible == 0) {
    for (i = 0x27; i < 0x2b; i++) {
      GfxSprite *sprite = ((GfxSprite **)screen->data)[i];
      sprite->flags &= ~1u;
    }
  } else {
    for (i = 0x27; i < 0x2b; i++) {
      if (i == 0x27) {
        UiSetButtonIcon(((GfxSprite **)screen->data)[i], 2);
      } else if (i == 0x28) {
        UiSetButtonIcon(((GfxSprite **)screen->data)[i], 1);
      }
      ((GfxSprite **)screen->data)[i]->flags |= 1;
      ((GfxSprite **)screen->data)[i]->alpha = 1.0f;
    }
  }
}
