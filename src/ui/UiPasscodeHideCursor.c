// bdc 0x0893e43c UiPasscodeHideCursor
#include "bdc.h"

/* Hides the cursor sprites 0 and 0x25 of `UiPasscode`. */

void UiPasscodeHideCursor(UiPasscode *screen)

{
  GfxSprite **sprites;

  sprites = (GfxSprite **)screen->base.data;
  sprites[0]->flags = sprites[0]->flags & 0xfffffffe;
  sprites = (GfxSprite **)screen->base.data;
  sprites[0x25]->flags = sprites[0x25]->flags & 0xfffffffe;
}
