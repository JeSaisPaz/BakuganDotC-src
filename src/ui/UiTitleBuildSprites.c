// bdc 0x08950f8c UiTitleBuildSprites
#include "bdc.h"

/* Builds the sprites of `UiTitle`: creates the layout 0 sprites into the
   `screen->data` sprite array, loads the `"dialog_moji"` boot texture (`GfxBootTextureReplace`),
   clears the alpha of sprites 0, 1 and 5, shows cell (0, 2) on sprite 1, places sprites 2/3 at
   Y 192/224, clears flag bit0 of sprites 4 and 6, centres sprites 2–4 (scale 1, angle 0,
   alpha 0, UV inset 0.5) and sets the animation angle to π/2. */

void UiTitleBuildSprites(UiScreen *screen)
{
  UiTitle *self = (UiTitle *)screen;
  int i;

  UiLayoutCreateSprites(screen->spriteLayer, screen->data, 0);
  GfxBootTextureReplace("dialog_moji", NULL, 0);
  ((GfxSprite **)screen->data)[0]->alpha = 0.0f;
  ((GfxSprite **)screen->data)[1]->alpha = 0.0f;
  GfxSpriteSetCell(((GfxSprite **)screen->data)[1], 0.0f, 2.0f);
  ((GfxSprite **)screen->data)[5]->alpha = 0.0f;
  ((GfxSprite **)screen->data)[2]->posY = 192.0f;
  ((GfxSprite **)screen->data)[3]->posY = 224.0f;
  ((GfxSprite **)screen->data)[4]->flags &= ~1u;
  ((GfxSprite **)screen->data)[6]->flags &= ~1u;
  for (i = 2; i < 5; i++) {
    GfxSpriteCenterPivot(((GfxSprite **)screen->data)[i]);
    GfxSpriteSetScaleRotation(((GfxSprite **)screen->data)[i], 1.0f, 1.0f, 0.0f, false);
    ((GfxSprite **)screen->data)[i]->alpha = 0.0f;
    GfxSpriteInsetUv(0.5f, ((GfxSprite **)screen->data)[i]);
  }
  self->animAngle = 1.5707964f;
}
